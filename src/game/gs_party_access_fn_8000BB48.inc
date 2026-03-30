    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000BB6C
    li r3, -0x1
    b @L_8000BBDC
    @L_8000BB6C:
    lbz r0, lbl_80478840(r13)
    cmplwi r0, 0x0
    beq @L_8000BB8C
    lis r3, lbl_803A1A48@ha
    addi r3, r3, lbl_803A1A48@l
    bl fn_80124A60
    li r0, 0x0
    stb r0, lbl_80478840(r13)
    @L_8000BB8C:
    lis r3, lbl_803A1A48@ha
    li r4, 0x0
    addi r3, r3, lbl_803A1A48@l
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_800096B4
    cmpwi r3, 0x0
    bge @L_8000BBBC
    li r3, -0x1
    b @L_8000BBDC
    @L_8000BBBC:
    lis r4, lbl_803A1A48@ha
    li r3, 0x0
    addi r4, r4, lbl_803A1A48@l
    li r5, 0x0
    li r6, 0x4
    li r7, 0x1
    bl fn_80129F20
    li r3, -0x1
    @L_8000BBDC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
