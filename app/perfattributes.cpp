/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd
** All rights reserved.
** For any questions to The Qt Company, please use contact form at http://www.qt.io/contact-us
**
** This file is part of the Qt Enterprise Perf Profiler Add-on.
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation and appearing in
** the file LICENSE.GPLv3 included in the packaging of this file. Please
** review the following information to ensure the GNU General Public License
** requirements will be met: https://www.gnu.org/licenses/gpl.html.
**
** If you have questions regarding the use of this file, please use
** contact form at http://www.qt.io/contact-us
**
****************************************************************************/

#include "perfattributes.h"
#include "perfdata.h"
#include <QDebug>

PerfEventAttributes::PerfEventAttributes()
{
    memset(this, 0, sizeof(PerfEventAttributes));
}

QDataStream &operator>>(QDataStream &stream, PerfEventAttributes &attrs)
{
    quint64 flags;
    stream >> attrs.m_type >> attrs.m_size;

    if (attrs.m_size < sizeof(PerfEventAttributes)) {
        qWarning() << "unsupported file format";
        return stream;
    }

    stream >> attrs.m_config >> attrs.m_samplePeriod >> attrs.m_sampleType >> attrs.m_readFormat
           >> flags >> attrs.m_wakeupEvents >> attrs.m_bpType >> attrs.m_bpAddr >> attrs.m_bpLen
           >> attrs.m_branchSampleType >> attrs.m_sampleRegsUser >> attrs.m_sampleStackUser
           >> attrs.m_reserved2;

    if (static_cast<QSysInfo::Endian>(stream.byteOrder()) != QSysInfo::ByteOrder) {
        // bit fields are saved in byte order; who came up with that BS?
        quint64 newFlags = 0;
        for (int i = 0; i < 64; ++i) {
            if ((flags & (1ull << i)) != 0)
                newFlags |= (1ull << (i / 8 + 7 - (i % 8)));
        }
        flags = newFlags;
    }

    *(&attrs.m_readFormat + 1) = flags;

    stream.skipRawData(attrs.m_size - sizeof(PerfEventAttributes));

    return stream;
}

int PerfEventAttributes::sampleIdOffset() const
{
    int offset = 0;

    if (m_sampleType & SAMPLE_IDENTIFIER)
        return 0;

    if (!(m_sampleType & SAMPLE_ID))
        return -1;

    if (m_sampleType & SAMPLE_IP)
        offset += sizeof(quint64); // PerfRecordSample::m_ip

    if (m_sampleType & SAMPLE_TID)
        offset += sizeof(quint32) + sizeof(quint32); // PerfRecordSampleId::{m_pid|m_tid}

    if (m_sampleType & SAMPLE_TIME)
        offset += sizeof(quint64); // PerfSampleId::m_time

    if (m_sampleType & SAMPLE_ADDR)
        offset += sizeof(quint64); // PerfRecordSample::m_addr

    return offset;
}




bool PerfAttributes::read(QIODevice *device, PerfHeader *header)
{
    if (header->attrSize() < sizeof(PerfEventAttributes) + sizeof(PerfFileSection)) {
        qWarning() << "unsupported file format";
        return false;
    }

    PerfEventAttributes attrs;
    PerfFileSection ids;

    for (uint i = 0; i < header->numAttrs(); ++i) {

        if (!device->seek(header->attrs().offset + header->attrSize() * i)) {
            qWarning() << "cannot seek to attribute section" << i
                       << header->attrs().offset + header->attrSize() * i;
            return false;
        }

        QDataStream stream(device);
        stream.setByteOrder(header->byteOrder());
        stream >> attrs;

        if (attrs.size() < sizeof(PerfEventAttributes))
            return false;

        if (i == 0)
            m_globalAttributes = attrs;

        stream >> ids;
        if (ids.size > 0) {
            if (!device->seek(ids.offset)) {
                qWarning() << "cannot seek to attribute ID section";
                return false;
            }

            QDataStream idStream(device);
            stream.setByteOrder(header->byteOrder());
            quint64 id;
            for (uint i = 0; i < ids.size / sizeof(quint64); ++i) {
                idStream >> id;
                m_attributes[id] = attrs;
            }
        }

    }
    return true;
}

void PerfAttributes::setGlobalAttributes(const PerfEventAttributes &attributes)
{
    m_globalAttributes = attributes;
}

void PerfAttributes::addAttributes(quint64 id, const PerfEventAttributes &attributes)
{
    m_attributes[id] = attributes;
}

const PerfEventAttributes &PerfAttributes::attributes(quint64 id) const
{
    QHash<quint64, PerfEventAttributes>::ConstIterator i = m_attributes.find(id);
    if (i != m_attributes.end())
        return i.value();
    else
        return m_globalAttributes;
}