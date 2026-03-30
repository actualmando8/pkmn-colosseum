    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    clrlwi. r0, r5, 16
    stmw r29, 0x14(r1)
    mr r29, r5
    mr r30, r6
    mr r31, r7
    beq @L_801F76A4
    cmplwi r0, 0xa
    blt @L_801F7628
    b @L_801F76A4
    @L_801F7628:
    cmplwi r0, 0x4
    bge @L_801F7638
    mr r3, r4
    bl fn_801F7870
    @L_801F7638:
    cmplwi r3, 0x0
    beq @L_801F76A4
    clrlwi r0, r29, 16
    cmplwi r0, 0x8
    bgt @L_801F76A4
    lis r4, jumptable_80375628@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_80375628@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    clrlwi r4, r31, 16
    bl fn_801F77E0
    b @L_801F76A4
    mr r5, r31
    clrlwi r4, r30, 24
    bl fn_801F77BC
    b @L_801F76A4
    mr r5, r31
    clrlwi r4, r30, 24
    bl fn_801F7798
    b @L_801F76A4
    clrlwi r4, r31, 16
    bl fn_801F78AC
    b @L_801F76A4
    clrlwi r4, r31, 24
    bl fn_801F789C
    @L_801F76A4:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
