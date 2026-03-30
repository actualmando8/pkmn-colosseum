    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    li r30, 0x0
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    b @L_800DC848
    @L_800DC704:
    lwz r0, lbl_8047AAEC(r13)
    add r29, r0, r31
    lbz r0, 0x0(r29)
    cmplwi r0, 0x1
    bne @L_800DC840
    lbz r0, 0x3(r29)
    cmplwi r0, 0x1
    bne @L_800DC840
    lwz r3, 0xc(r29)
    lfs f1, 0x68(r29)
    bl fn_801A6370
    lwz r3, 0xc(r29)
    bl fn_801A6408
    lis r3, 0x4330
    lbz r0, 0x71(r29)
    stw r28, 0xc(r1)
    extsb r0, r0
    lfd f2, lbl_8047CA80(r2)
    stw r3, 0x8(r1)
    cmpwi r0, -0x1
    lfs f3, 0x64(r29)
    lfd f0, 0x8(r1)
    lfs f1, 0x6c(r29)
    fsubs f2, f0, f2
    lfs f0, lbl_8047CA70(r2)
    lwz r3, 0x5c(r29)
    fsubs f4, f1, f0
    fmuls f1, f3, f2
    bne @L_800DC788
    lfs f0, 0x68(r29)
    fsubs f0, f0, f1
    stfs f0, 0x68(r29)
    b @L_800DC79C
    @L_800DC788:
    cmpwi r0, 0x1
    bne @L_800DC79C
    lfs f0, 0x68(r29)
    fadds f0, f0, f1
    stfs f0, 0x68(r29)
    @L_800DC79C:
    cmpwi r3, 0x1
    beq @L_800DC7F0
    bge @L_800DC7B4
    cmpwi r3, 0x0
    bge @L_800DC7C0
    b @L_800DC840
    @L_800DC7B4:
    cmpwi r3, 0x3
    bge @L_800DC840
    b @L_800DC80C
    @L_800DC7C0:
    lfs f0, lbl_8047CA74(r2)
    lfs f1, 0x68(r29)
    fsubs f0, f4, f0
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_800DC840
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x70(r29)
    stb r0, 0x71(r29)
    stfs f0, 0x68(r29)
    b @L_800DC840
    @L_800DC7F0:
    lfs f0, 0x68(r29)
    fcmpo cr0, f0, f4
    cror eq, gt, eq
    bne @L_800DC840
    fsubs f0, f0, f4
    stfs f0, 0x68(r29)
    b @L_800DC840
    @L_800DC80C:
    lfs f1, 0x68(r29)
    fcmpo cr0, f1, f4
    cror eq, gt, eq
    bne @L_800DC828
    li r0, -0x1
    stb r0, 0x71(r29)
    b @L_800DC840
    @L_800DC828:
    lfs f0, lbl_8047CA78(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_800DC840
    li r0, 0x1
    stb r0, 0x71(r29)
    @L_800DC840:
    addi r31, r31, 0x74
    addi r30, r30, 0x1
    @L_800DC848:
    lwz r0, lbl_8047AAF0(r13)
    cmplw r30, r0
    blt @L_800DC704
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
