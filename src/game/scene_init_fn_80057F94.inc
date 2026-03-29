    nofralloc
    stwu r1, -0xf0(r1)
    mflr r0
    stw r0, 0xf4(r1)
    stw r31, 0xec(r1)
    stw r30, 0xe8(r1)
    mr r30, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80057FC4
    li r3, 0x0
    b @L_80058138
    @L_80057FC4:
    mr r3, r30
    bl fn_8011F5C8
    mr r31, r3
    clrlwi r0, r31, 16
    cmplwi r0, 0x0
    bne @L_80057FE4
    li r31, -0x1
    b @L_800580FC
    @L_80057FE4:
    cmplwi r0, 0xc9
    bne @L_800580AC
    lis r3, lbl_802676F0@ha
    li r0, 0x1c
    addi r3, r3, lbl_802676F0@l
    addi r5, r1, 0x4
    subi r4, r3, 0x4
    mtctr r0
    @L_80058004:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80058004
    mr r3, r30
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    bl fn_80122334
    mr r31, r3
    clrlwi r0, r31, 24
    cmplwi r0, 0x1c
    blt @L_80058048
    li r3, -0x1
    b @L_800580F8
    @L_80058048:
    mr r3, r30
    bl fn_8011F5C8
    mr r4, r3
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    bne @L_80058068
    li r3, -0x1
    b @L_800580F8
    @L_80058068:
    mr r3, r30
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80058098
    clrlwi r0, r31, 24
    addi r3, r1, 0xc
    slwi r0, r0, 3
    lwzx r3, r3, r0
    b @L_800580F8
    @L_80058098:
    clrlwi r0, r31, 24
    addi r3, r1, 0x8
    slwi r0, r0, 3
    lwzx r3, r3, r0
    b @L_800580F8
    @L_800580AC:
    mr r3, r30
    mr r4, r31
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    mr r4, r31
    cmplwi r0, 0x0
    li r3, 0x0
    li r5, 0x5a
    beq @L_800580E0
    li r0, 0x1
    b @L_800580E4
    @L_800580E0:
    li r0, 0x0
    @L_800580E4:
    clrlwi r6, r0, 16
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_800580F8
    li r3, -0x1
    @L_800580F8:
    mr r31, r3
    @L_800580FC:
    addis r0, r31, 0x1
    cmplwi r0, 0xffff
    bne @L_80058110
    li r3, 0x0
    b @L_80058138
    @L_80058110:
    mr r4, r31
    li r3, 0x48a
    bl fn_8017B07C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80058134
    mr r3, r31
    bl fn_800F92D4
    b @L_80058138
    @L_80058134:
    li r3, 0x0
    @L_80058138:
    lwz r0, 0xf4(r1)
    lwz r31, 0xec(r1)
    lwz r30, 0xe8(r1)
    mtlr r0
    addi r1, r1, 0xf0
    blr
