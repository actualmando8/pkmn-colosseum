    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r5
    mr r31, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EBC8
    li r3, 0x1
    b @L_8022EC2C
@L_8022EBC8:
    mr r3, r31
    bl fn_8022EC40
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022EC28
    lis r4, lbl_80379F58@ha
    subi r0, r3, 0x1
    addi r3, r4, lbl_80379F58@l
    mr r7, r31
    addis r3, r3, 0x1
    li r4, 0x0
    stb r0, 0x609b(r3)
    li r3, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_803796D7@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803796D7@l
    li r5, 0x0
    bl fn_80211B94
    cmplwi r30, 0x0
    beq @L_8022EC28
    li r0, 0x1
    stb r0, 0x0(r30)
@L_8022EC28:
    li r3, 0x1
@L_8022EC2C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
