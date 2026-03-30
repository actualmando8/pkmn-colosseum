    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lis r3, 0x91a3
    lwz r4, lbl_8047AE84(r13)
    subi r0, r3, 0x4c3b
    lis r3, 0x8889
    mulhwu r0, r0, r4
    li r29, 0x0
    subi r3, r3, 0x7777
    srwi r30, r0, 11
    mulli r0, r30, 0xe10
    cmplwi r30, 0x64
    subf r0, r0, r4
    mulhwu r0, r3, r0
    srwi r31, r0, 5
    blt @L_801318D8
    lis r3, 0x51ec
    lis r5, lbl_80427030@ha
    subi r0, r3, 0x7ae1
    li r4, 0x10
    mulhwu r0, r0, r30
    addi r3, r5, lbl_80427030@l
    li r6, 0x0
    srwi r5, r0, 5
    bl fn_80132834
    lis r4, 0x51ec
    stw r3, lbl_8047AEA8(r13)
    subi r0, r4, 0x7ae1
    li r5, 0x0
    mulhwu r0, r0, r30
    lis r4, lbl_80427050@ha
    lhz r6, 0x0(r3)
    slwi r5, r5, 1
    addi r3, r4, lbl_80427050@l
    sthx r6, r3, r5
    srwi r0, r0, 5
    li r29, 0x1
    mulli r0, r0, 0x64
    subf r30, r0, r30
    @L_801318D8:
    lis r3, 0xcccd
    lis r5, lbl_80427030@ha
    subi r0, r3, 0x3333
    li r4, 0x10
    mulhwu r0, r0, r30
    addi r3, r5, lbl_80427030@l
    li r6, 0x0
    srwi r5, r0, 3
    bl fn_80132834
    lis r4, 0xcccd
    mr r6, r29
    subi r0, r4, 0x3333
    stw r3, lbl_8047AEA8(r13)
    mulhwu r0, r0, r30
    lis r5, lbl_80427050@ha
    lhz r7, 0x0(r3)
    lis r4, lbl_80427030@ha
    slwi r6, r6, 1
    addi r5, r5, lbl_80427050@l
    srwi r0, r0, 3
    sthx r7, r5, r6
    mulli r0, r0, 0xa
    addi r3, r4, lbl_80427030@l
    addi r29, r29, 0x1
    li r4, 0x10
    subf r5, r0, r30
    li r6, 0x0
    bl fn_80132834
    lis r4, 0xcccd
    mr r6, r29
    stw r3, lbl_8047AEA8(r13)
    subi r0, r4, 0x3333
    mulhwu r0, r0, r31
    addi r29, r29, 0x1
    lhz r8, 0x0(r3)
    lis r5, lbl_80427050@ha
    slwi r7, r6, 1
    addi r6, r5, lbl_80427050@l
    mr r4, r29
    lis r3, lbl_80427030@ha
    sthx r8, r6, r7
    slwi r4, r4, 1
    li r5, 0x3a
    addi r3, r3, lbl_80427030@l
    sthx r5, r6, r4
    addi r29, r29, 0x1
    srwi r5, r0, 3
    li r4, 0x10
    li r6, 0x0
    bl fn_80132834
    lis r4, 0xcccd
    mr r6, r29
    subi r0, r4, 0x3333
    stw r3, lbl_8047AEA8(r13)
    mulhwu r0, r0, r31
    lis r5, lbl_80427050@ha
    lhz r7, 0x0(r3)
    lis r4, lbl_80427030@ha
    slwi r6, r6, 1
    addi r5, r5, lbl_80427050@l
    srwi r0, r0, 3
    sthx r7, r5, r6
    mulli r0, r0, 0xa
    addi r3, r4, lbl_80427030@l
    addi r29, r29, 0x1
    li r4, 0x10
    subf r5, r0, r31
    li r6, 0x0
    bl fn_80132834
    stw r3, lbl_8047AEA8(r13)
    mr r0, r29
    lis r4, lbl_80427050@ha
    addi r29, r29, 0x1
    lhz r5, 0x0(r3)
    addi r3, r4, lbl_80427050@l
    slwi r4, r0, 1
    slwi r0, r29, 1
    sthx r5, r3, r4
    li r4, 0x0
    sthx r4, r3, r0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
