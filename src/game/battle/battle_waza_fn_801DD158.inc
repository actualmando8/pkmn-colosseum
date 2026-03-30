    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r3, 0x24(r3)
    cmplwi r3, 0x0
    beq @L_801DD1A0
    lbz r0, 0x18(r30)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801DD198
    li r4, 0x1
    bl fn_800E4014
    b @L_801DD1A0
@L_801DD198:
    li r4, 0x0
    bl fn_800E4014
@L_801DD1A0:
    lwz r31, 0x68(r30)
    b @L_801DD21C
@L_801DD1A8:
    lbz r0, 0x14(r31)
    lwz r30, 0x34(r31)
    cmplwi r0, 0x0
    beq @L_801DD218
    lbz r0, 0x15(r31)
    extsb r0, r0
    cmpwi r0, -0x1
    bne @L_801DD1D4
    mr r3, r31
    bl fn_801DBB10
    b @L_801DD218
@L_801DD1D4:
    mr r3, r31
    bl fn_801DB988
    extsb. r0, r3
    bne @L_801DD200
    lbz r0, 0x15(r31)
    extsb r0, r0
    cmpwi r0, -0x1
    beq @L_801DD218
    mr r3, r31
    bl fn_801DBB10
    b @L_801DD218
@L_801DD200:
    extsb. r0, r3
    bge @L_801DD218
    mr r3, r31
    bl fn_801DBB10
    mr r3, r31
    bl fn_801DBDDC
@L_801DD218:
    mr r31, r30
@L_801DD21C:
    cmplwi r31, 0x0
    bne @L_801DD1A8
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
