    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    lwz r0, lbl_8047A424(r13)
    li r3, 0x0
    lis r4, lbl_803A2518@ha
    stw r3, lbl_8047A40C(r13)
    addi r31, r4, lbl_803A2518@l
    clrlwi r5, r0, 16
    li r29, 0x0
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    lwz r0, lbl_8047A420(r13)
    mr r28, r3
    addi r3, r31, 0x170
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r0, r3
    mr r3, r28
    mr r30, r0
    addi r6, r1, 0xc
    addi r7, r1, 0x18
    li r4, 0x2
    li r5, 0x0
    bl fn_80128A64
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8002DFA4
    cmplwi r0, 0xffff
    beq @L_8002DFA4
    li r29, 0x1
    @L_8002DFA4:
    mr r3, r30
    addi r6, r1, 0xc
    addi r7, r1, 0x18
    li r4, 0x2
    li r5, 0x0
    bl fn_80128A64
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8002DFE4
    cmplwi r0, 0xffff
    beq @L_8002DFE4
    mr r3, r30
    li r29, 0x1
    bl fn_8011F5C8
    clrlwi r0, r3, 16
    stw r0, lbl_8047A40C(r13)
    @L_8002DFE4:
    clrlwi r0, r29, 24
    cmplwi r0, 0x1
    bne @L_8002E244
    li r0, 0x0
    lfs f1, lbl_8047B9D0(r2)
    stb r0, lbl_8047A41C(r13)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0xde
    bl fn_80102510
    addi r3, r31, 0xd18
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    lwz r3, lbl_8047A418(r13)
    bl fn_801CB9D8
    li r3, 0x0
    bl fn_80112260
    bl fn_800F0308
    lfs f1, lbl_8047B9D4(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    mr r3, r28
    addi r6, r1, 0xa
    addi r7, r1, 0x14
    li r4, 0x2
    li r5, 0x0
    bl fn_80128A64
    mr r4, r3
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    beq @L_8002E0BC
    cmplwi r0, 0xffff
    beq @L_8002E0BC
    li r0, 0x1
    mr r3, r28
    stb r0, lbl_8047A408(r13)
    addi r6, r1, 0x14
    li r5, 0x0
    li r7, 0x0
    li r8, 0x1
    li r9, 0x0
    li r10, 0x1
    bl fn_8012805C
    cmpwi r3, 0x2
    bne @L_8002E0B4
    li r0, 0x0
    stb r0, lbl_8047A408(r13)
    @L_8002E0B4:
    li r0, 0x0
    stb r0, lbl_8047A408(r13)
    @L_8002E0BC:
    mr r3, r30
    addi r6, r1, 0x8
    addi r7, r1, 0x10
    li r4, 0x2
    li r5, 0x0
    bl fn_80128A64
    mr r4, r3
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    beq @L_8002E12C
    cmplwi r0, 0xffff
    beq @L_8002E12C
    li r0, 0x1
    mr r3, r30
    stb r0, lbl_8047A408(r13)
    addi r6, r1, 0x10
    li r5, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1
    bl fn_8012805C
    cmpwi r3, 0x2
    bne @L_8002E124
    li r0, 0x0
    stb r0, lbl_8047A408(r13)
    @L_8002E124:
    li r0, 0x0
    stb r0, lbl_8047A408(r13)
    @L_8002E12C:
    lfs f1, lbl_8047B9D4(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
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
    mr r29, r3
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
    bl fn_80113F48
    lis r4, 0xffe
    mr r28, r3
    addi r3, r4, 0x1000
    bl fn_801CBA0C
    mr r4, r3
    mr r3, r28
    stw r4, lbl_8047A418(r13)
    bl fn_800F9318
    stw r3, lbl_8047A414(r13)
    lis r3, 0xfff
    addi r4, r3, 0x1800
    li r3, 0x37c
    li r5, 0x0
    li r6, 0x1
    bl fn_80176E0C
    li r3, 0x4
    bl fn_80177A44
    lwz r3, lbl_8047A414(r13)
    li r4, 0x1
    bl fn_800E4014
    li r3, 0x0
    bl fn_80112260
    li r3, 0xde
    li r4, 0x1
    bl fn_8010264C
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r0, 0x1
    stb r0, lbl_8047A41C(r13)
    @L_8002E244:
    li r0, 0x12
    stw r0, lbl_8047A42C(r13)
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
