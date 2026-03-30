    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    addi r31, r3, 0x0
    stw r30, 0x28(r1)
    addi r30, r5, 0x0
    stw r29, 0x24(r1)
    addi r29, r4, 0x0
    addi r4, r1, 0x18
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B5260
    b @L_800B531C
    @L_800B5260:
    lwz r3, 0x18(r1)
    addi r4, r29, 0x0
    addi r5, r1, 0x14
    bl fn_800B4338
    mr. r4, r3
    bge @L_800B5284
    lwz r3, 0x18(r1)
    bl fn_800B02F4
    b @L_800B531C
    @L_800B5284:
    lwz r3, 0x18(r1)
    lwz r4, 0x14(r1)
    bl fn_800B463C
    cmpwi r3, 0x0
    beq @L_800B52A8
    lwz r3, 0x18(r1)
    li r4, -0x1
    bl fn_800B02F4
    b @L_800B531C
    @L_800B52A8:
    lwz r3, 0x18(r1)
    bl fn_800B1E0C
    lwz r0, 0x14(r1)
    li r4, 0xff
    lwz r6, 0x18(r1)
    li r5, 0x40
    slwi r0, r0, 6
    add r3, r3, r0
    lhz r0, 0x36(r3)
    sth r0, 0xbe(r6)
    bl memset
    cmplwi r30, 0x0
    beq @L_800B52E4
    mr r0, r30
    b @L_800B52EC
    @L_800B52E4:
    lis r3, fn_800AF280@ha
    addi r0, r3, fn_800AF280@l
    @L_800B52EC:
    lwz r5, 0x18(r1)
    lis r3, fn_800B5184@ha
    addi r4, r3, fn_800B5184@l
    stw r0, 0xd0(r5)
    mr r3, r31
    bl fn_800B1FAC
    mr. r31, r3
    bge @L_800B5318
    lwz r3, 0x18(r1)
    mr r4, r31
    bl fn_800B02F4
    @L_800B5318:
    mr r3, r31
    @L_800B531C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
