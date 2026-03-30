    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    rlwinm. r0, r3, 0, 16, 31
    stmw r27, 0xc(r1)
    mr r27, r3
    bne @L_8020DAF4
    li r3, 0x0
    b @L_8020DD30
@L_8020DAF4:
    bl fn_8020E0F8
    mr r0, r3
    li r3, 0x0
    mr r29, r0
    bl fn_801EF62C
    li r3, 0x9b0
    bl fn_801903B0
    mr r3, r27
    bl fn_801EF61C
    bl fn_800FF56C
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x4a
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r29
    bl fn_8020E0B0
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x2
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    bl fn_801D23C0
    mr r3, r29
    bl fn_8020E0E0
    rlwinm r3, r3, 0, 24, 31
    bl fn_8020E488
    cmplwi r3, 0x0
    beq @L_8020DBCC
    bl fn_8020E260
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020DBCC
    mr r3, r29
    li r4, 0x1
    bl fn_8020E068
    bl fn_801FCCC4
    cmplwi r3, 0x0
    beq @L_8020DBCC
    bl fn_801FCC7C
    mr r28, r3
    bl fn_80261388
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8020DBC0
    bl fn_80261444
    rlwinm. r3, r3, 0, 16, 31
    beq @L_8020DBC0
    bl fn_80190528
@L_8020DBC0:
    mr r4, r28
    li r3, 0x59
    bl fn_80132A38
@L_8020DBCC:
    li r3, 0x1
    li r4, 0x3e8
    li r5, 0xff
    bl fn_80165A20
    li r3, 0x3e8
    bl fn_801657D0
    mr r3, r29
    bl fn_8020DFF0
    bl fn_8020DEB0
    mr r28, r3
    bl fn_8020DE80
    mr r30, r3
    mr r3, r28
    bl fn_8020DE98
    mr r31, r3
    mr r3, r28
    bl fn_8020DE68
    fmr f2, f1
    lfs f1, -20856(r2)
    mr r4, r31
    mr r5, r30
    li r3, 0x9
    bl fn_801C4164
    mr r3, r28
    bl fn_8020DE50
    rlwinm. r3, r3, 0, 16, 31
    beq @L_8020DC44
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
@L_8020DC44:
    bl fn_801EF7B4
    mr r3, r27
    bl fn_800FF730
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    bl fn_800F0308
    mr r3, r27
    bl fn_8011395C
    mr r3, r29
    bl fn_8020DFD8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020DCF0
    bl fn_801EF634
    mr r4, r3
    li r3, 0x0
    bl fn_801F1DBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020DCF0
    li r3, 0x0
    bl fn_801EF61C
    li r3, 0xe05
    bl fn_801903B0
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    mr r0, r3
    li r3, 0x0
    srawi r0, r0, 1
    addze r4, r0
    bl fn_80129474
    li r3, 0x1
    bl fn_801D0AFC
    bl fn_8018DA88
    bl fn_80113FE8
    lis r4, 0x596
    li r3, 0x0
    addi r4, r4, 0x8
    bl fn_8011288C
    bl fn_800F0308
    bl fn_801EF634
    b @L_8020DD30
@L_8020DCF0:
    li r3, 0x9b0
    bl fn_80190528
    bl fn_80112700
    bl fn_801140C8
    li r3, 0xe05
    bl fn_801902E0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020DD24
    lfs f1, -20852(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
@L_8020DD24:
    li r3, 0x0
    bl fn_801EF61C
    bl fn_801EF634
@L_8020DD30:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
