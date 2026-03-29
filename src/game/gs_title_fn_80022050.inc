    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r4
    li r30, 0x0
    li r31, 0x0
    @L_80022074:
    mr r3, r31
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_80014118
    lwz r3, 0xc(r1)
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002209C
    addi r30, r30, 0x1
    @L_8002209C:
    addi r31, r31, 0x1
    cmpwi r31, 0x6
    blt @L_80022074
    cmpwi r30, 0x6
    blt @L_80022110
    bl fn_801347E0
    extsb r30, r3
    li r31, 0x0
    b @L_800220DC
    @L_800220C0:
    extsb r4, r31
    li r3, 0x0
    bl fn_801347E8
    extsb r0, r3
    cmpwi r0, 0x0
    bgt @L_800220E4
    addi r31, r31, 0x1
    @L_800220DC:
    cmpw r31, r30
    blt @L_800220C0
    @L_800220E4:
    cmpw r31, r30
    blt @L_80022110
    li r3, 0x2
    li r4, 0x4416
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    b @L_80022160
    @L_80022110:
    addi r3, r1, 0x14
    addi r4, r1, 0x10
    bl fn_800140FC
    lwz r3, 0x10(r1)
    bl fn_801F7EF0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80022154
    li r3, 0x2
    li r4, 0x426d
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    b @L_80022160
    @L_80022154:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r29)
    @L_80022160:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
