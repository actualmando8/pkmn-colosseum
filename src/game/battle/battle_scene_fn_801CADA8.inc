    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    li r31, 0x0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r0, r30, 0, 24, 31
    cmpwi r0, 0x4
    beq @L_801CAE38
    bge @L_801CADF8
    cmpwi r0, 0x2
    beq @L_801CAE18
    bge @L_801CAE28
    cmpwi r0, 0x1
    bge @L_801CAE08
    b @L_801CAE64
@L_801CADF8:
    cmpwi r0, 0x6
    beq @L_801CAE58
    bge @L_801CAE64
    b @L_801CAE48
@L_801CAE08:
    li r4, 0xfe
    bl fn_80130BB0
    mr r31, r3
    b @L_801CAE64
@L_801CAE18:
    li r4, 0xfe
    bl fn_80130A88
    mr r31, r3
    b @L_801CAE64
@L_801CAE28:
    li r4, 0xfe
    bl fn_801309A0
    mr r31, r3
    b @L_801CAE64
@L_801CAE38:
    li r4, 0xff
    bl fn_80130890
    mr r31, r3
    b @L_801CAE64
@L_801CAE48:
    li r4, 0xff
    bl fn_80130770
    mr r31, r3
    b @L_801CAE64
@L_801CAE58:
    li r4, 0xff
    bl fn_80130660
    mr r31, r3
@L_801CAE64:
    mr r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
