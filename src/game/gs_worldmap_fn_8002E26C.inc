    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lis r3, lbl_803A2518@ha
    addi r31, r3, lbl_803A2518@l
    addi r3, r31, 0x0
    bl fn_80124A60
    lwz r0, lbl_8047A424(r13)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    lwz r0, lbl_8047A420(r13)
    mr r30, r3
    addi r3, r31, 0x170
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r0, r3
    addi r3, r31, 0x0
    mr r29, r0
    mr r4, r29
    bl fn_8011F5FC
    mr r3, r29
    mr r4, r30
    bl fn_8011F5FC
    mr r3, r30
    addi r4, r31, 0x0
    bl fn_8011F5FC
    li r3, 0xde
    bl fn_80102510
    addi r3, r31, 0xd18
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    li r0, 0x0
    lwz r3, lbl_8047A414(r13)
    stb r0, lbl_8047A41C(r13)
    li r4, 0x0
    bl fn_800E4014
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x1
    bl fn_801024E8
    bl fn_80113F48
    lis r4, 0x10b6
    li r5, 0x0
    addi r4, r4, 0x1800
    li r6, 0x0
    bl fn_80176E0C
    bl fn_800F0308
    li r3, 0x4c8
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r3, 0x1
    bl fn_80112260
    lis r3, 0x10b1
    li r4, 0x0
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x1
    bl fn_80176B48
    lfs f1, lbl_8047B9D8(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x0
    bl fn_80112260
    addi r3, r31, 0xd18
    li r4, 0xe8
    li r5, 0x11c
    bl fn_8010A5BC
    addi r3, r31, 0xcd0
    li r4, 0xe8
    li r5, 0x11c
    bl fn_8010A5BC
    mr r4, r30
    addi r3, r31, 0xd18
    bl fn_80109C88
    mr r4, r29
    addi r3, r31, 0xcd0
    bl fn_80109C88
    lis r4, 0xfff
    li r3, 0x37c
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x1
    bl fn_80176E0C
    lwz r3, lbl_8047A414(r13)
    li r4, 0x1
    bl fn_800E4014
    li r3, 0xde
    li r4, 0x1
    bl fn_8010264C
    li r0, 0x1
    lfs f1, lbl_8047B9D8(r2)
    stb r0, lbl_8047A41C(r13)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r0, 0x10
    stw r0, lbl_8047A42C(r13)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
