    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_8013FEE0
    lwz r31, 0x10(r30)
    cmplwi r31, 0x0
    bne @L_8013FE0C
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    bne @L_8013FE0C
    li r3, 0x0
    b @L_8013FEF4
    @L_8013FE0C:
    lwz r3, 0x14(r30)
    cmplwi r3, 0x0
    beq @L_8013FE28
    lhz r4, 0x1e(r30)
    li r5, 0x1
    bl fn_801190DC
    stw r3, 0x20(r30)
    @L_8013FE28:
    cmplwi r31, 0x0
    beq @L_8013FED8
    mr r3, r31
    li r4, 0x1
    bl fn_800E4014
    mr r3, r31
    li r4, 0x1
    bl fn_800E3CC8
    mr r3, r31
    bl fn_800EC1BC
    clrlwi. r0, r3, 24
    beq @L_8013FE90
    lhz r4, 0x1c(r30)
    mr r3, r31
    bl fn_800ECCA8
    lwz r4, 0x18(r30)
    mr r3, r31
    bl fn_800ECB74
    lfs f1, lbl_8047D328(r2)
    mr r3, r31
    bl fn_800EC9DC
    lfs f1, lbl_8047D32C(r2)
    mr r3, r31
    bl fn_800ECA78
    mr r3, r31
    bl fn_800EC990
    @L_8013FE90:
    mr r3, r31
    bl fn_800EC1B0
    clrlwi. r0, r3, 24
    beq @L_8013FED8
    lhz r4, 0x1c(r30)
    mr r3, r31
    bl fn_800EC35C
    lwz r4, 0x18(r30)
    mr r3, r31
    bl fn_800EC208
    lfs f1, lbl_8047D328(r2)
    mr r3, r31
    bl fn_800EC308
    lfs f1, lbl_8047D32C(r2)
    mr r3, r31
    bl fn_800EC2A4
    mr r3, r31
    bl fn_800EC1E4
    @L_8013FED8:
    li r3, 0x1
    b @L_8013FEF4
    @L_8013FEE0:
    lis r3, lbl_80273078@ha
    addi r3, r3, lbl_80273078@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013FEF4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
