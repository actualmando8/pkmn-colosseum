    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r28, 0x30(r1)
    mr. r30, r3
    mr r31, r4
    beq @L_801F4AAC
    li r4, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_801F54A4
    li r4, 0x1
    bl fn_8011B950
    mr r3, r30
    li r4, 0x0
    li r5, 0xc
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x35
    li r6, 0x0
    bl fn_801F54A4
    li r4, 0x2
    bl fn_801F7530
    lis r3, lbl_80279C28@ha
    li r0, 0x3
    addi r3, r3, lbl_80279C28@l
    addi r5, r1, 0x4
    subi r4, r3, 0x4
    mtctr r0
    @L_801F48F8:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801F48F8
    lhz r0, 0x4(r4)
    addi r29, r1, 0x8
    li r28, 0x0
    sth r0, 0x4(r5)
    b @L_801F4940
    @L_801F4920:
    clrlslwi r0, r28, 16, 1
    mr r3, r30
    lhzx r5, r29, r0
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    addi r28, r28, 0x1
    @L_801F4940:
    clrlwi r0, r28, 16
    cmplwi r0, 0xd
    blt @L_801F4920
    mr r3, r30
    li r4, 0x0
    li r5, 0x50
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x51
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x52
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x53
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x54
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x55
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x57
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x58
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x5a
    li r6, 0x0
    bl fn_801F54A4
    cmplwi r3, 0x0
    beq @L_801F4A64
    li r5, 0x0
    mr r4, r5
    b @L_801F4A58
    @L_801F4A4C:
    clrlslwi r0, r5, 16, 2
    addi r5, r5, 0x1
    stwx r4, r3, r0
    @L_801F4A58:
    clrlwi r0, r5, 16
    cmplwi r0, 0x8
    blt @L_801F4A4C
    @L_801F4A64:
    mr r3, r30
    li r4, 0x0
    li r5, 0x5b
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    mr r3, r30
    clrlwi r7, r31, 16
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0xc
    li r6, 0x0
    li r7, 0x1
    bl fn_801F4C14
    @L_801F4AAC:
    lmw r28, 0x30(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
