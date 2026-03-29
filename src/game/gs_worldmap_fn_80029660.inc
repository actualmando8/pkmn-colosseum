    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    cmpwi r29, 0x2
    li r31, 0x1
    beq @L_800296A8
    bge @L_8002969C
    cmpwi r29, 0x0
    bge @L_800296EC
    b @L_800296E8
    @L_8002969C:
    cmpwi r29, 0x4
    bge @L_800296E8
    b @L_800296D0
    @L_800296A8:
    clrlwi r5, r30, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800296EC
    li r31, 0x0
    b @L_800296EC
    @L_800296D0:
    cmpwi r30, 0x0
    blt @L_800296E0
    cmpwi r30, 0x3
    blt @L_800296EC
    @L_800296E0:
    li r31, 0x0
    b @L_800296EC
    @L_800296E8:
    li r31, 0x0
    @L_800296EC:
    cmpwi r31, 0x0
    bne @L_800296FC
    li r3, 0x0
    b @L_80029744
    @L_800296FC:
    lis r3, lbl_803A2068@ha
    li r4, 0x0
    addi r31, r3, lbl_803A2068@l
    li r0, 0x1
    sth r4, 0x0(r31)
    li r3, 0x390
    stw r29, 0x18(r31)
    stw r30, 0x1c(r31)
    stw r4, 0x20(r31)
    stw r4, 0x24(r31)
    stw r0, 0x28(r31)
    bl fn_800FF730
    lis r4, 0x596
    li r3, 0x0
    addi r4, r4, 0x8
    bl fn_8011288C
    bl fn_800F0308
    lwz r3, 0x20(r31)
    @L_80029744:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
