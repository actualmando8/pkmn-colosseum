    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    clrlwi. r0, r5, 16
    stmw r30, 0x8(r1)
    mr r30, r5
    mr r31, r6
    beq @L_801F76E0
    cmplwi r0, 0xa
    blt @L_801F76E8
    @L_801F76E0:
    li r3, 0x0
    b @L_801F7784
    @L_801F76E8:
    cmplwi r0, 0x4
    bge @L_801F76F8
    mr r3, r4
    bl fn_801F7870
    @L_801F76F8:
    cmplwi r3, 0x0
    bne @L_801F7708
    li r3, 0x0
    b @L_801F7784
    @L_801F7708:
    clrlwi r0, r30, 16
    cmplwi r0, 0x8
    bgt @L_801F7780
    lis r4, jumptable_8037564C@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_8037564C@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    bl fn_801F7858
    clrlwi r3, r3, 16
    b @L_801F7784
    clrlwi r4, r31, 24
    bl fn_801F7824
    b @L_801F7784
    clrlwi r4, r31, 24
    bl fn_801F77F0
    b @L_801F7784
    bl fn_801F793C
    clrlwi r3, r3, 16
    b @L_801F7784
    mr r4, r31
    bl fn_801F7908
    b @L_801F7784
    mr r4, r31
    bl fn_801F78D4
    b @L_801F7784
    bl fn_801F78BC
    clrlwi r3, r3, 24
    b @L_801F7784
    @L_801F7780:
    li r3, 0x0
    @L_801F7784:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
