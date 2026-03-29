    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    stw r0, 0x104(r1)
    stmw r27, 0xec(r1)
    mr r27, r3
    addi r3, r1, 0xc
    bl fn_8012C540
    mr r29, r3
    addi r30, r1, 0xc
    li r28, 0x0
    b @L_8000D750
    @L_8000D73C:
    lhz r4, 0x30(r30)
    li r3, 0x4
    bl fn_80116D30
    addi r30, r30, 0x34
    addi r28, r28, 0x1
    @L_8000D750:
    cmpw r28, r29
    blt @L_8000D73C
    bl fn_8012BCA4
    bl fn_801D23C0
    bl fn_800F0308
    bl fn_800EC918
    li r0, 0x1
    stb r0, lbl_8047A2A0(r13)
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xe4(r1)
    lfd f2, lbl_8047B6F8(r2)
    stw r0, 0xe0(r1)
    lfs f0, lbl_8047B6F0(r2)
    lfd f1, 0xe0(r1)
    fsubs f1, f1, f2
    fdivs f1, f0, f1
    bl fn_8010206C
    b @L_8000D7A4
    @L_8000D7A0:
    bl fn_800F0308
    @L_8000D7A4:
    bl fn_80102014
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000D7A0
    li r3, 0x55
    li r4, 0x1f4
    li r5, 0x1
    li r6, 0x1
    bl fn_801661D0
    li r3, 0x1
    bl fn_800D3074
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8000D7E8
    li r30, 0x41
    b @L_8000D7EC
    @L_8000D7E8:
    li r30, 0x109
    @L_8000D7EC:
    clrlwi r0, r27, 24
    cmpwi r0, 0x1
    beq @L_8000D9DC
    bge @L_8000D808
    cmpwi r0, 0x0
    bge @L_8000D810
    b @L_8000DA20
    @L_8000D808:
    cmpwi r0, 0x3
    b @L_8000DA20
    @L_8000D810:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8000D930
    lis r3, 0x8001
    addi r29, r3, -0x2550
    @L_8000D828:
    li r3, 0x1
    bl fn_800D3074
    stw r30, lbl_8047A2A4(r13)
    bl fn_800FF560
    stw r3, lbl_8047A2A8(r13)
    bl fn_80130CD8
    bl fn_800FE6F8
    lwz r3, lbl_8047A2A8(r13)
    bl fn_800F03D4
    lwz r4, lbl_8047A2A8(r13)
    mr r8, r29
    li r3, 0xf
    li r5, 0x4000
    subi r4, r4, 0x20
    li r6, 0x1
    stw r4, lbl_8047A2AC(r13)
    li r7, 0x1
    bl fn_800F07A8
    bl fn_800F0308
    bl fn_80130CD8
    bl fn_800FE6DC
    lwz r0, lbl_8047A2B0(r13)
    cmpwi r0, 0x0
    bne @L_8000D8AC
    mr r3, r30
    bl fn_80102510
    bl fn_800EC8DC
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    bl fn_800EC918
    b @L_8000D828
    @L_8000D8AC:
    cmpwi r0, 0x1
    bne @L_8000D8CC
    mr r3, r30
    bl fn_80102510
    bl fn_800EC8DC
    bl fn_8004BE0C
    bl fn_800EC918
    b @L_8000D828
    @L_8000D8CC:
    cmpwi r0, 0x2
    bne @L_8000DA20
    mr r3, r30
    bl fn_80102510
    bl fn_800EC8DC
    addi r4, r1, 0x8
    li r3, 0x0
    li r5, 0x0
    bl fn_80018F88
    mr r31, r3
    bl fn_800EC918
    clrlwi r0, r31, 16
    cmplwi r0, 0x0
    beq @L_8000D828
    mr r3, r31
    bl fn_80019070
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_8000DA20
    lwz r5, 0x8(r1)
    clrlwi r4, r31, 16
    li r6, 0x0
    li r7, 0x0
    bl fn_8012BAD0
    b @L_8000DA20
    @L_8000D930:
    lis r3, 0x8001
    addi r29, r3, -0x2550
    @L_8000D938:
    li r3, 0x1
    bl fn_800D3074
    stw r30, lbl_8047A2A4(r13)
    bl fn_800FF560
    stw r3, lbl_8047A2A8(r13)
    bl fn_80130CD8
    bl fn_800FE6F8
    lwz r3, lbl_8047A2A8(r13)
    bl fn_800F03D4
    lwz r4, lbl_8047A2A8(r13)
    mr r8, r29
    li r3, 0xf
    li r5, 0x4000
    subi r4, r4, 0x20
    li r6, 0x1
    stw r4, lbl_8047A2AC(r13)
    li r7, 0x1
    bl fn_800F07A8
    bl fn_800F0308
    bl fn_80130CD8
    bl fn_800FE6DC
    lwz r0, lbl_8047A2B0(r13)
    cmpwi r0, 0x0
    bne @L_8000D9B4
    mr r3, r30
    bl fn_80102510
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    b @L_8000D938
    @L_8000D9B4:
    cmpwi r0, 0x1
    bne @L_8000DA20
    bl fn_801CBAB8
    cmpwi r3, 0x1
    bne @L_8000D938
    bl fn_80109764
    li r0, 0x0
    stb r0, lbl_8047A2A0(r13)
    bl fn_800F0308
    b @L_8000D938
    @L_8000D9DC:
    addi r4, r1, 0x8
    li r3, 0x0
    li r5, 0x0
    bl fn_80018F88
    mr r31, r3
    clrlwi r0, r31, 16
    cmplwi r0, 0x0
    beq @L_8000DA20
    bl fn_80019070
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_8000DA20
    lwz r5, 0x8(r1)
    clrlwi r4, r31, 16
    li r6, 0x0
    li r7, 0x0
    bl fn_8012BAD0
    @L_8000DA20:
    mr r3, r31
    bl fn_80019070
    li r3, 0x1f4
    li r4, 0x1
    li r5, 0x1
    bl fn_801660D8
    mr r3, r30
    bl fn_80102510
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xe4(r1)
    lfd f2, lbl_8047B6F8(r2)
    stw r0, 0xe0(r1)
    lfs f0, lbl_8047B6F0(r2)
    lfd f1, 0xe0(r1)
    fsubs f1, f1, f2
    fdivs f1, f0, f1
    bl fn_80102038
    mr r3, r30
    li r4, 0x1
    bl fn_80102428
    li r3, 0x2
    bl fn_800D3074
    bl fn_8012BBA8
    bl fn_800EC8DC
    li r0, 0x0
    li r3, 0x0
    stb r0, lbl_8047A2A0(r13)
    lmw r27, 0xec(r1)
    lwz r0, 0x104(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
