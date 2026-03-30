    nofralloc
    mflr r0
    li r5, 0x4
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    mulli r4, r30, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r4
    addi r3, r30, 0x0
    li r4, 0x0
    bl EXISelect
    cmpwi r3, 0x0
    bne @L_800AF8F0
    mr r3, r30
    bl EXIUnlock
    li r3, -0x3
    b @L_800AFAB4
    @L_800AF8F0:
    addi r3, r31, 0xe0
    bl fn_8009A590
    lbz r0, 0x94(r31)
    cmpwi r0, 0xf3
    beq @L_800AF9C0
    bge @L_800AF918
    cmpwi r0, 0xf1
    beq @L_800AF95C
    bge @L_800AF924
    b @L_800AF9C0
    @L_800AF918:
    cmpwi r0, 0xf5
    bge @L_800AF9C0
    b @L_800AF95C
    @L_800AF924:
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    lis r4, 0x1062
    lis r3, fn_800AF7FC@ha
    srwi r0, r0, 2
    addi r4, r4, 0x4dd3
    mulhwu r0, r4, r0
    srwi r0, r0, 6
    mulli r6, r0, 0x64
    addi r7, r3, fn_800AF7FC@l
    addi r3, r31, 0xe0
    li r5, 0x0
    bl OSSetAlarm
    b @L_800AF9C0
    @L_800AF95C:
    lis r3, 0x8000
    lwz r4, 0xc(r31)
    lwz r0, 0xf8(r3)
    lis r3, fn_800AF7FC@ha
    srawi r9, r4, 13
    srwi r7, r0, 2
    li r0, 0x2
    li r4, 0x0
    mullw r8, r4, r0
    mulhwu r6, r7, r0
    add r8, r8, r6
    mullw r5, r7, r0
    addze r9, r9
    mullw r6, r7, r4
    srawi r0, r9, 31
    mullw r4, r0, r5
    mulhwu r0, r9, r5
    addi r7, r3, fn_800AF7FC@l
    add r3, r8, r6
    add r4, r4, r0
    mullw r0, r9, r3
    mullw r6, r9, r5
    addi r3, r31, 0xe0
    add r5, r4, r0
    bl OSSetAlarm
    @L_800AF9C0:
    lwz r5, 0xa0(r31)
    addi r3, r30, 0x0
    addi r4, r31, 0x94
    li r6, 0x1
    bl fn_80098368
    cmpwi r3, 0x0
    bne @L_800AF9F4
    mr r3, r30
    bl EXIDeselect
    mr r3, r30
    bl EXIUnlock
    li r3, -0x3
    b @L_800AFAB4
    @L_800AF9F4:
    lbz r0, 0x94(r31)
    cmplwi r0, 0x52
    bne @L_800AFA38
    lwz r4, 0x80(r31)
    mr r3, r30
    lwz r5, 0x14(r31)
    li r6, 0x1
    addi r4, r4, 0x200
    bl fn_80098368
    cmpwi r3, 0x0
    bne @L_800AFA38
    mr r3, r30
    bl EXIDeselect
    mr r3, r30
    bl EXIUnlock
    li r3, -0x3
    b @L_800AFAB4
    @L_800AFA38:
    lwz r3, 0xa4(r31)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_800AFA60
    mr r3, r30
    bl EXIDeselect
    mr r3, r30
    bl EXIUnlock
    li r3, 0x0
    b @L_800AFAB4
    @L_800AFA60:
    lbz r0, 0x94(r31)
    cmplwi r0, 0x52
    bne @L_800AFA74
    li r5, 0x200
    b @L_800AFA78
    @L_800AFA74:
    li r5, 0x80
    @L_800AFA78:
    lis r3, fn_800AF474@ha
    lwz r4, 0xb4(r31)
    addi r7, r3, fn_800AF474@l
    lwz r6, 0xa4(r31)
    mr r3, r30
    bl EXIDma
    cmpwi r3, 0x0
    bne @L_800AFAB0
    mr r3, r30
    bl EXIDeselect
    mr r3, r30
    bl EXIUnlock
    li r3, -0x3
    b @L_800AFAB4
    @L_800AFAB0:
    li r3, 0x0
    @L_800AFAB4:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
