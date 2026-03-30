    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0x2
    li r6, 0x18
    stw r0, 0x14(r1)
    li r0, 0x0
    stw r31, 0xc(r1)
    lwz r4, -21580(r13)
    lwz r31, 0x50(r4)
    subi r4, r3, 0x2028
    addis r5, r31, 0x2
    stw r0, 0xc(r31)
    mr r3, r31
    subi r5, r5, 0x2014
    bl fn_801CBCDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801CF948
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x3
    beq @L_801CF8C8
    lwz r0, 0x4(r31)
    stw r0, 0x2c(r3)
    lwz r3, -21580(r13)
    lwz r4, 0x30(r3)
    lwz r0, 0x2c(r3)
    cmpw r4, r0
    ble @L_801CF8C8
    stw r4, 0x2c(r3)
    li r0, 0x5
    lwz r3, -21580(r13)
    stw r0, 0x4(r3)
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x3
    bge @L_801CF8B8
    cmpwi r0, 0x1
    bge @L_801CF880
    b @L_801CF8B8
@L_801CF880:
    lwz r3, 0x58(r3)
    li r0, 0x3bfa
    addi r4, r31, 0x4
    subi r5, r3, 0x4
    mtctr r0
@L_801CF894:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CF894
    lwz r3, -21580(r13)
    li r0, 0x25
    stw r0, 0x10(r3)
    b @L_801CF8C0
@L_801CF8B8:
    li r0, 0x24
    stw r0, 0x10(r3)
@L_801CF8C0:
    li r3, 0x30
    b @L_801CF9B4
@L_801CF8C8:
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x4
    bge @L_801CF940
    cmpwi r0, 0x1
    bge @L_801CF8E4
    b @L_801CF940
@L_801CF8E4:
    lwz r3, 0x58(r3)
    li r0, 0x3bfa
    addi r4, r31, 0x4
    subi r5, r3, 0x4
    mtctr r0
@L_801CF8F8:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CF8F8
    lwz r4, -21580(r13)
    li r5, 0xc
    li r0, 0x3
    li r3, 0x2c
    stw r5, 0x4(r4)
    lwz r4, -21580(r13)
    stw r0, 0x8(r4)
    lwz r4, -21580(r13)
    lbz r0, 0x40(r4)
    cmplwi r0, 0x0
    beq @L_801CF9B4
    li r3, 0x11
    b @L_801CF9B4
@L_801CF940:
    li r3, 0x24
    b @L_801CF9B4
@L_801CF948:
    lwz r5, -21580(r13)
    li r0, 0x0
    li r3, 0x1f
    lwz r4, 0x20(r5)
    lwz r5, 0x50(r5)
    rlwinm r4, r4, 9, 0, 22
    addis r4, r4, 0x2
    add r6, r4, r5
    stw r0, -0x1ffc(r6)
    stw r0, -0x1ff4(r6)
    lwz r5, -0x2000(r6)
    lwz r0, -0x1ffc(r6)
    lwz r4, -0x1ff8(r6)
    add r5, r5, r0
    lwz r0, -0x1ff4(r6)
    add r5, r5, r4
    lwz r4, -0x1ff0(r6)
    add r5, r5, r0
    lwz r0, -0x1fec(r6)
    add r5, r5, r4
    lwz r4, -0x1fe8(r6)
    add r5, r5, r0
    lwz r0, -0x1fe4(r6)
    add r5, r5, r4
    add r5, r5, r0
    neg r0, r5
    stw r0, -0x1ff4(r6)
@L_801CF9B4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
