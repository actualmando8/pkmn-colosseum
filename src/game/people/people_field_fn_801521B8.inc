    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    addi r11, r1, 0x20
    bl _savegpr_27
    lis r5, lbl_80445EF8@ha
    lis r6, lbl_8043CCF8@ha
    addi r28, r5, lbl_80445EF8@l
    mr r30, r4
    lis r5, fn_801521A8@ha
    sth r3, 0x0(r28)
    addi r27, r6, lbl_8043CCF8@l
    li r31, 0x0
    addi r29, r5, fn_801521A8@l
    b @L_801522B8
    @L_801521F4:
    lwz r4, 0x0(r27)
    mr r3, r28
    lhz r5, 0x8(r27)
    mr r7, r29
    li r6, 0x20
    bl fn_80162118
    cmplwi r3, 0x0
    stw r3, lbl_8047AF88(r13)
    beq @L_801522B0
    lhz r0, 0x2(r3)
    cmplwi r0, 0xffff
    beq @L_801522B0
    addi r3, r3, 0xc
    li r0, 0x0
    stw r3, lbl_8047AF84(r13)
    lwz r3, 0x0(r3)
    stw r3, 0x0(r30)
    lwz r3, lbl_8047AF88(r13)
    lwz r3, 0x8(r3)
    stw r3, 0x4(r30)
    stw r0, 0xc(r30)
    lwz r3, lbl_8047AF84(r13)
    lwz r0, 0x8(r3)
    stw r0, 0x14(r30)
    lwz r3, lbl_8047AF84(r13)
    lwz r0, 0x4(r3)
    clrlwi r0, r0, 8
    stw r0, 0x10(r30)
    lwz r3, lbl_8047AF84(r13)
    lwz r0, 0xc(r3)
    stw r0, 0x18(r30)
    lwz r3, lbl_8047AF84(r13)
    lwz r0, 0x4(r3)
    srwi r0, r0, 24
    stb r0, 0x1c(r30)
    lwz r3, lbl_8047AF88(r13)
    lwz r4, 0x1c(r3)
    cmplwi r4, 0x0
    beq @L_801522A8
    mulli r0, r31, 0xc
    lis r3, lbl_8043CCF8@ha
    addi r3, r3, lbl_8043CCF8@l
    lwzx r0, r3, r0
    add r0, r4, r0
    stw r0, 0x8(r30)
    @L_801522A8:
    li r3, 0x0
    b @L_801522C8
    @L_801522B0:
    addi r27, r27, 0xc
    addi r31, r31, 0x1
    @L_801522B8:
    lhz r0, lbl_8047AFAA(r13)
    cmpw r31, r0
    blt @L_801521F4
    li r3, -0x1
    @L_801522C8:
    addi r11, r1, 0x20
    bl _restgpr_27
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
