import qbs

ElfUtilsStaticLib {
    name: "ebl"
    files: [
        "ebl-hooks.h",
        "ebl_check_special_section.c",
        "ebl_check_special_symbol.c",
        "ebl_syscall_abi.c",
        "eblabicfi.c",
        "eblauxvinfo.c",
        "eblbackendname.c",
        "eblbsspltp.c",
        "eblcheckobjattr.c",
        "eblcheckreloctargettype.c",
        "eblclosebackend.c",
        "eblcopyrelocp.c",
        "eblcorenote.c",
        "eblcorenotetypename.c",
        "ebldebugscnp.c",
        "ebldwarftoregno.c",
        "ebldynamictagcheck.c",
        "ebldynamictagname.c",
        "eblelfclass.c",
        "eblelfdata.c",
        "eblelfmachine.c",
        "eblgotpcreloccheck.c",
        "eblgstrtab.c",
        "eblinitreg.c",
        "eblmachineflagcheck.c",
        "eblmachineflagname.c",
        "eblmachinesectionflagcheck.c",
        "eblnonerelocp.c",
        "eblnormalizepc.c",
        "eblobjnote.c",
        "eblobjnotetypename.c",
        "eblopenbackend.c",
        "eblosabiname.c",
        "eblreginfo.c",
        "eblrelativerelocp.c",
        "eblrelocsimpletype.c",
        "eblreloctypecheck.c",
        "eblreloctypename.c",
        "eblrelocvaliduse.c",
        "eblresolvesym.c",
        "eblretval.c",
        "eblsectionname.c",
        "eblsectionstripp.c",
        "eblsectiontypename.c",
        "eblsegmenttypename.c",
        "eblstother.c",
        "eblstrtab.c",
        "eblsymbolbindingname.c",
        "eblsymboltypename.c",
        "eblsysvhashentrysize.c",
        "eblunwind.c",
        "eblwstrtab.c",
        "libebl.h",
        "libeblP.h",
    ]
}
