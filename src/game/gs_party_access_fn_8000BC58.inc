    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000BC7C
    li r3, -0x1
    b @L_8000BCB4
    @L_8000BC7C:
    li r3, 0x0
    li r4, 0x3
    li r5, 0x4
    bl fn_8012A5B0
    cmplwi r3, 0x0
    bne @L_8000BC9C
    li r3, -0x1
    b @L_8000BCB4
    @L_8000BC9C:
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_800096B4
    @L_8000BCB4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
