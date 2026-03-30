    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r5
    mr r27, r4
    mr r29, r6
    mr r30, r7
    mr r31, r8
    mr r6, r27
    li r5, 0x0
    b @L_800DA7B8
    @L_800DA720:
    cmplwi r0, 0x19
    bgt @L_800DA7B4
    lis r4, jumptable_803152B8@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_803152B8@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    ori r5, r5, 0x1
    b @L_800DA7B4
    ori r5, r5, 0x40
    b @L_800DA7B4
    ori r5, r5, 0x2
    b @L_800DA7B4
    ori r5, r5, 0x4
    b @L_800DA7B4
    ori r5, r5, 0x8
    b @L_800DA7B4
    ori r5, r5, 0x10
    b @L_800DA7B4
    ori r5, r5, 0x20
    b @L_800DA7B4
    ori r5, r5, 0x80
    b @L_800DA7B4
    ori r5, r5, 0x100
    b @L_800DA7B4
    ori r5, r5, 0x200
    b @L_800DA7B4
    ori r5, r5, 0x400
    b @L_800DA7B4
    ori r5, r5, 0x800
    b @L_800DA7B4
    ori r5, r5, 0x1000
    b @L_800DA7B4
    ori r5, r5, 0x2000
    b @L_800DA7B4
    ori r5, r5, 0x4000
    @L_800DA7B4:
    addi r6, r6, 0x18
    @L_800DA7B8:
    lwz r0, 0x0(r6)
    cmpwi r0, 0xff
    bne @L_800DA720
    lwz r12, 0x0(r30)
    cmplwi r12, 0x0
    beq @L_800DA844
    mr r4, r29
    mr r6, r31
    mtctr r12
    bctrl
    b @L_800DA844
    @L_800DA7E4:
    lwz r12, 0x4(r30)
    cmplwi r12, 0x0
    beq @L_800DA7FC
    mr r3, r31
    mtctr r12
    bctrl
    @L_800DA7FC:
    mr r26, r27
    b @L_800DA820
    @L_800DA804:
    mr r3, r26
    mr r4, r28
    mr r5, r30
    mr r6, r31
    bl fn_800DA880
    mr r28, r3
    addi r26, r26, 0x18
    @L_800DA820:
    lwz r0, 0x0(r26)
    cmpwi r0, 0xff
    bne @L_800DA804
    lwz r12, 0xc(r30)
    cmplwi r12, 0x0
    beq @L_800DA844
    mr r3, r31
    mtctr r12
    bctrl
    @L_800DA844:
    clrlwi. r0, r29, 16
    subi r29, r29, 0x1
    bne @L_800DA7E4
    lwz r12, 0x10(r30)
    cmplwi r12, 0x0
    beq @L_800DA868
    mr r3, r31
    mtctr r12
    bctrl
    @L_800DA868:
    mr r3, r28
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
