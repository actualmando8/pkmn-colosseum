    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    lwz r0, lbl_8047A8D8(r13)
    cmpwi r0, 0x1
    beq @L_800AC594
    lwz r3, lbl_80478A28(r13)
    bl OSRegisterVersion
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    lis r3, 0x431c
    lis r4, 0x1
    srwi r0, r0, 2
    subi r3, r3, 0x217d
    mulhwu r0, r3, r0
    srwi r9, r0, 15
    subi r5, r4, 0x5bd8
    subi r3, r4, 0x5bf0
    subi r0, r4, 0x9e8
    lis r4, 0x1062
    mullw r7, r9, r5
    addi r10, r4, 0x4dd3
    mullw r5, r9, r3
    mullw r4, r9, r0
    mulli r8, r9, 0x7b24
    mulli r3, r9, 0xbb8
    mulhwu r8, r10, r8
    mulhwu r7, r10, r7
    mulhwu r5, r10, r5
    mulhwu r4, r10, r4
    mulhwu r3, r10, r3
    srwi r8, r8, 9
    srwi r7, r7, 9
    stw r8, lbl_8047A8E4(r13)
    srwi r5, r5, 9
    srwi r4, r4, 9
    stw r7, lbl_8047A8EC(r13)
    li r31, 0x0
    srwi r3, r3, 9
    stw r5, lbl_8047A8F4(r13)
    lis r6, 0xcc00
    stw r3, lbl_8047A904(r13)
    li r3, 0x1
    lwz r0, 0x6c00(r6)
    stw r4, lbl_8047A8FC(r13)
    rlwinm r0, r0, 0, 27, 25
    ori r0, r0, 0x20
    stw r31, lbl_8047A8E0(r13)
    stw r31, lbl_8047A8E8(r13)
    stw r31, lbl_8047A8F0(r13)
    stw r31, lbl_8047A8F8(r13)
    stw r31, lbl_8047A900(r13)
    lwz r5, 0x6c04(r6)
    stw r0, 0x6c00(r6)
    rlwinm r0, r5, 0, 24, 15
    nop
    stw r0, 0x6c04(r6)
    lwz r0, 0x6c04(r6)
    clrrwi r0, r0, 8
    nop
    stw r0, 0x6c04(r6)
    stw r31, 0x6c0c(r6)
    bl fn_800AC304
    li r3, 0x0
    bl fn_800AC210
    lis r3, fn_800AC628@ha
    stw r31, lbl_8047A8C8(r13)
    addi r4, r3, fn_800AC628@l
    stw r31, lbl_8047A8CC(r13)
    li r3, 0x5
    stw r30, lbl_8047A8D0(r13)
    bl __OSSetInterruptHandler
    lis r3, 0x400
    bl __OSUnmaskInterrupts
    lis r3, fn_800AC5AC@ha
    addi r4, r3, fn_800AC5AC@l
    li r3, 0x8
    bl __OSSetInterruptHandler
    lis r3, 0x80
    bl __OSUnmaskInterrupts
    li r0, 0x1
    stw r0, lbl_8047A8D8(r13)
    @L_800AC594:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
