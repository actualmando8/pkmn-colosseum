    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r4, lbl_80279C28@ha
    stw r0, 0x44(r1)
    addi r4, r4, lbl_80279C28@l
    li r0, 0x3
    addi r5, r1, 0x4
    stmw r29, 0x34(r1)
    mr r29, r3
    subi r4, r4, 0x4
    mtctr r0
    @L_801F4AEC:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801F4AEC
    lhz r0, 0x4(r4)
    addi r31, r1, 0x8
    li r30, 0x0
    sth r0, 0x4(r5)
    b @L_801F4B34
    @L_801F4B14:
    clrlslwi r0, r30, 16, 1
    mr r3, r29
    lhzx r5, r31, r0
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    addi r30, r30, 0x1
    @L_801F4B34:
    clrlwi r0, r30, 16
    cmplwi r0, 0xd
    blt @L_801F4B14
    mr r3, r29
    li r4, 0x0
    li r5, 0x50
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x0
    li r5, 0x51
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x0
    li r5, 0x52
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x0
    li r5, 0x53
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x0
    li r5, 0x54
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x0
    li r5, 0x55
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x0
    li r5, 0x57
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    lmw r29, 0x34(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
