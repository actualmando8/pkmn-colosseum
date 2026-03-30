    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r8, 0x0
    mr r11, r4
    stw r0, 0x14(r1)
    mr r10, r5
    mr r9, r6
    mr r4, r8
    b @L_801F1D8C
    @L_801F1D80:
    clrlslwi r0, r8, 16, 2
    addi r8, r8, 0x1
    stwx r4, r7, r0
    @L_801F1D8C:
    clrlwi r0, r8, 16
    cmplwi r0, 0x8
    blt @L_801F1D80
    mr r4, r7
    mr r5, r11
    mr r6, r10
    mr r7, r9
    bl fn_801F61EC
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
