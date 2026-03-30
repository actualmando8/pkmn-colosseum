    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    bl fn_80113F48
    mr r4, r27
    mr r31, r3
    bl fn_8018D998
    cmplwi r3, 0x0
    beq @L_801CB888
    mr r3, r31
    mr r4, r27
    mr r5, r28
    mr r6, r29
    rlwinm r7, r30, 0, 24, 31
    bl fn_8018B368
    b @L_801CB940
@L_801CB888:
    mr r3, r31
    mr r4, r27
    bl fn_800F9318
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_801CB940
    mr r4, r28
    bl fn_800ECCA8
    xoris r3, r29, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    mr r3, r31
    lfd f1, -21832(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f1, f0, f1
    bl fn_800ECA78
    lfs f1, -21840(r2)
    mr r3, r31
    bl fn_800EC9DC
    mr r3, r31
    mr r4, r28
    bl fn_800EC35C
    xoris r3, r29, 0x8000
    lis r0, 0x4330
    stw r3, 0x14(r1)
    mr r3, r31
    lfd f1, -21832(r2)
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f1, f0, f1
    bl fn_800EC2A4
    lfs f1, -21840(r2)
    mr r3, r31
    bl fn_800EC308
    cmpwi r30, 0x0
    beq @L_801CB92C
    mr r3, r31
    li r4, 0x1
    bl fn_800ECB74
    b @L_801CB938
@L_801CB92C:
    mr r3, r31
    li r4, 0x0
    bl fn_800ECB74
@L_801CB938:
    mr r3, r31
    bl fn_800EC990
@L_801CB940:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
