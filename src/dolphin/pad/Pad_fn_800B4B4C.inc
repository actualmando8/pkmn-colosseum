    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stmw r27, 0x14(r1)
    addi r29, r3, 0x0
    lis r3, lbl_803FC620@ha
    mulli r5, r29, 0x110
    addi r0, r3, lbl_803FC620@l
    mr. r30, r4
    add r31, r0, r5
    blt @L_800B4C3C
    lwz r28, 0xc0(r31)
    lwz r4, 0xc(r28)
    cmpwi r4, 0x0
    bge @L_800B4B90
    li r30, -0xe
    b @L_800B4C3C
    @L_800B4B90:
    lwz r5, 0xc(r31)
    lwz r6, 0x8(r28)
    subi r0, r5, 0x1
    nor r3, r0, r0
    add r0, r6, r5
    and r0, r3, r0
    subf r27, r6, r0
    subf r0, r27, r4
    stw r0, 0xc(r28)
    lwz r0, 0xc(r28)
    cmpwi r0, 0x0
    ble @L_800B4C3C
    mr r3, r31
    bl fn_800B1A08
    lwz r0, 0x8(r28)
    add r0, r0, r27
    stw r0, 0x8(r28)
    lhz r0, 0x10(r28)
    slwi r0, r0, 1
    lhzx r0, r3, r0
    sth r0, 0x10(r28)
    lhz r3, 0x10(r28)
    cmplwi r3, 0x5
    blt @L_800B4BFC
    lhz r0, 0x10(r31)
    cmplw r3, r0
    blt @L_800B4C04
    @L_800B4BFC:
    li r30, -0x6
    b @L_800B4C3C
    @L_800B4C04:
    lwz r5, 0xc(r28)
    lwz r0, 0xc(r31)
    cmpw r5, r0
    bge @L_800B4C18
    b @L_800B4C1C
    @L_800B4C18:
    mr r5, r0
    @L_800B4C1C:
    mullw r4, r0, r3
    lwz r6, 0xb4(r31)
    lis r3, fn_800B4B4C@ha
    addi r7, r3, fn_800B4B4C@l
    addi r3, r29, 0x0
    bl fn_800B1864
    mr. r30, r3
    bge @L_800B4C68
    @L_800B4C3C:
    lwz r27, 0xd0(r31)
    li r0, 0x0
    addi r3, r31, 0x0
    stw r0, 0xd0(r31)
    mr r4, r30
    bl fn_800B02F4
    addi r12, r27, 0x0
    mtlr r12
    addi r3, r29, 0x0
    addi r4, r30, 0x0
    blrl
    @L_800B4C68:
    lmw r27, 0x14(r1)
    lwz r0, 0x2c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
