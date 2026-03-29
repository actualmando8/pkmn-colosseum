    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lfs f0, lbl_8047B814(r2)
    li r0, 0x0
    lis r3, 0xb54
    stw r0, lbl_8047A31C(r13)
    addi r3, r3, 0x1000
    li r31, 0x1
    stfs f0, lbl_8047A348(r13)
    bl fn_800F92D4
    mr r4, r3
    lfs f0, lbl_8047B814(r2)
    li r0, 0x0
    li r3, 0x0
    mr r30, r4
    stfs f0, lbl_8047A340(r13)
    li r4, 0x406
    stw r30, lbl_8047A324(r13)
    stw r0, lbl_8047A33C(r13)
    stw r0, lbl_8047A328(r13)
    bl fn_801662E8
    li r3, 0x46a
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
    lwz r0, lbl_8047A310(r13)
    cmpwi r0, 0x1
    bne @L_8001FDEC
    li r4, 0x7
    li r3, 0x0
    li r0, 0x1
    stw r4, lbl_8047A31C(r13)
    stw r3, lbl_8047A310(r13)
    stw r0, lbl_8047A320(r13)
    b @L_8001FDF4
    @L_8001FDEC:
    li r0, 0x0
    stw r0, lbl_8047A320(r13)
    @L_8001FDF4:
    li r3, 0x15
    li r4, 0x0
    bl fn_8010264C
    @L_8001FE00:
    bl fn_8017B1AC
    mr r29, r3
    cmpwi r29, 0xb
    beq @L_8001FE18
    cmpwi r29, 0x4
    bne @L_8001FE40
    @L_8001FE18:
    lwz r0, lbl_8047A318(r13)
    cmpwi r0, 0x0
    bne @L_8001FE38
    bl fn_800EC918
    bl fn_80176A94
    bl fn_8016557C
    li r0, 0x1
    stw r0, lbl_8047A318(r13)
    @L_8001FE38:
    bl fn_800F0308
    b @L_800202FC
    @L_8001FE40:
    lwz r0, lbl_8047A318(r13)
    cmpwi r0, 0x1
    bne @L_8001FE6C
    bl fn_80176A44
    li r3, 0x0
    li r4, 0x406
    bl fn_801662E8
    bl fn_80165548
    bl fn_800EC8DC
    li r0, 0x0
    stw r0, lbl_8047A318(r13)
    @L_8001FE6C:
    cmpwi r29, 0x5
    bne @L_8001FE9C
    lwz r0, lbl_8047A314(r13)
    cmpwi r0, 0x0
    bne @L_8001FE94
    bl fn_800EC918
    bl fn_80176A94
    bl fn_8016557C
    li r0, 0x1
    stw r0, lbl_8047A314(r13)
    @L_8001FE94:
    bl fn_800F0308
    b @L_800202FC
    @L_8001FE9C:
    lwz r0, lbl_8047A314(r13)
    cmpwi r0, 0x1
    bne @L_8001FEC8
    bl fn_80176A44
    li r3, 0x0
    li r4, 0x406
    bl fn_801662E8
    bl fn_80165548
    bl fn_800EC8DC
    li r0, 0x0
    stw r0, lbl_8047A314(r13)
    @L_8001FEC8:
    lwz r0, lbl_8047A31C(r13)
    cmpwi r0, 0x1e
    beq @L_800201F4
    bge @L_8001FF2C
    cmpwi r0, 0x4
    beq @L_80020098
    bge @L_8001FF08
    cmpwi r0, 0x1
    beq @L_8001FFF0
    bge @L_8001FEFC
    cmpwi r0, 0x0
    bge @L_8001FF74
    b @L_800202FC
    @L_8001FEFC:
    cmpwi r0, 0x3
    bge @L_80020064
    b @L_80020018
    @L_8001FF08:
    cmpwi r0, 0x7
    beq @L_8002010C
    bge @L_8001FF20
    cmpwi r0, 0x6
    bge @L_800200D8
    b @L_800200B0
    @L_8001FF20:
    cmpwi r0, 0x9
    bge @L_800202FC
    b @L_80020184
    @L_8001FF2C:
    cmpwi r0, 0xc8
    beq @L_80020274
    bge @L_8001FF60
    cmpwi r0, 0x28
    beq @L_800201E8
    bge @L_8001FF54
    cmpwi r0, 0x20
    beq @L_80020200
    bge @L_800202FC
    b @L_80020240
    @L_8001FF54:
    cmpwi r0, 0x2a
    bge @L_800202FC
    b @L_800202E0
    @L_8001FF60:
    cmpwi r0, 0x3e8
    beq @L_800202F8
    bge @L_800202FC
    cmpwi r0, 0xca
    b @L_800202FC
    @L_8001FF74:
    cmplwi r30, 0x0
    beq @L_8001FFA8
    mr r3, r30
    li r4, 0x0
    bl fn_800ECCA8
    lfs f1, lbl_8047B814(r2)
    mr r3, r30
    bl fn_800ECA78
    lfs f1, lbl_8047B810(r2)
    mr r3, r30
    bl fn_800EC9DC
    mr r3, r30
    bl fn_800EC990
    @L_8001FFA8:
    mr r3, r30
    li r4, 0x0
    bl fn_800ECB74
    lis r4, 0xb55
    li r3, 0x12
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    lfs f1, lbl_8047B810(r2)
    li r3, 0x4
    bl fn_801C41C8
    lwz r0, lbl_8047A31C(r13)
    cmpwi r0, 0x0
    bne @L_800202FC
    li r0, 0x1
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_8001FFF0:
    mr r3, r30
    bl fn_800EC960
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80020010
    li r0, 0x2
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_80020010:
    bl fn_800F0308
    b @L_800202FC
    @L_80020018:
    cmplwi r30, 0x0
    beq @L_8002004C
    mr r3, r30
    li r4, 0x1
    bl fn_800ECCA8
    lfs f1, lbl_8047B814(r2)
    mr r3, r30
    bl fn_800ECA78
    lfs f1, lbl_8047B810(r2)
    mr r3, r30
    bl fn_800EC9DC
    mr r3, r30
    bl fn_800EC990
    @L_8002004C:
    mr r3, r30
    li r4, 0x0
    bl fn_800ECB74
    li r0, 0x3
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_80020064:
    cmplwi r30, 0x0
    bne @L_80020078
    b @L_8002008C
    b @L_80020078
    @L_80020074:
    bl fn_800F0308
    @L_80020078:
    mr r3, r30
    bl fn_800EC960
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80020074
    @L_8002008C:
    li r0, 0x8
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_80020098:
    lfs f1, lbl_8047B810(r2)
    li r3, 0x5
    bl fn_801C41C8
    li r0, 0x6
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_800200B0:
    li r3, 0x0
    bl fn_801C40F0
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800200D0
    li r0, 0x6
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_800200D0:
    bl fn_800F0308
    b @L_800202FC
    @L_800200D8:
    lfs f31, lbl_8047B810(r2)
    b @L_800200F4
    @L_800200E0:
    lfs f1, lbl_8047A348(r13)
    lfs f0, lbl_8047A344(r13)
    fadds f0, f1, f0
    stfs f0, lbl_8047A348(r13)
    bl fn_800F0308
    @L_800200F4:
    lfs f0, lbl_8047A348(r13)
    fcmpo cr0, f0, f31
    blt @L_800200E0
    li r0, 0x7
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_8002010C:
    lis r4, 0xb56
    li r3, 0x12
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    cmplwi r30, 0x0
    beq @L_80020158
    mr r3, r30
    li r4, 0x2
    bl fn_800ECCA8
    lfs f1, lbl_8047B814(r2)
    mr r3, r30
    bl fn_800ECA78
    lfs f1, lbl_8047B810(r2)
    mr r3, r30
    bl fn_800EC9DC
    mr r3, r30
    bl fn_800EC990
    @L_80020158:
    mr r3, r30
    li r4, 0x1
    bl fn_800ECB74
    lfs f1, lbl_8047B810(r2)
    li r3, 0x4
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r0, 0x1e
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_80020184:
    lis r4, 0xb56
    li r3, 0x12
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    cmplwi r30, 0x0
    beq @L_800201D0
    mr r3, r30
    li r4, 0x2
    bl fn_800ECCA8
    lfs f1, lbl_8047B814(r2)
    mr r3, r30
    bl fn_800ECA78
    lfs f1, lbl_8047B810(r2)
    mr r3, r30
    bl fn_800EC9DC
    mr r3, r30
    bl fn_800EC990
    @L_800201D0:
    mr r3, r30
    li r4, 0x1
    bl fn_800ECB74
    li r0, 0x1e
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_800201E8:
    li r0, 0x3e8
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_800201F4:
    li r0, 0x20
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_80020200:
    bl fn_801046B8
    mr r4, r3
    li r3, 0x13
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, 0x0
    blt @L_800202FC
    lfs f0, lbl_8047B814(r2)
    li r0, 0x1f
    stw r0, lbl_8047A31C(r13)
    stfs f0, lbl_8047A348(r13)
    b @L_800202FC
    @L_80020240:
    lfs f31, lbl_8047B818(r2)
    b @L_8002025C
    @L_80020248:
    lfs f1, lbl_8047A348(r13)
    lfs f0, lbl_8047A344(r13)
    fadds f0, f1, f0
    stfs f0, lbl_8047A348(r13)
    bl fn_800F0308
    @L_8002025C:
    lfs f0, lbl_8047A348(r13)
    fcmpo cr0, f0, f31
    blt @L_80020248
    li r0, 0xc8
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_80020274:
    li r3, 0x1
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    cmpwi r3, -0x1
    bne @L_80020298
    li r0, 0x20
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_80020298:
    cmpwi r3, 0x3
    beq @L_800202AC
    li r3, 0x0
    li r4, 0x0
    bl fn_80128E38
    @L_800202AC:
    li r3, 0x0
    li r4, 0x4
    bl fn_80135168
    cmplwi r3, 0x0
    beq @L_800202CC
    li r3, 0x1
    bl fn_800216E0
    b @L_800202D4
    @L_800202CC:
    li r3, 0x0
    bl fn_800216E0
    @L_800202D4:
    li r0, 0x29
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_800202E0:
    li r3, 0x3a1
    li r4, 0x0
    bl fn_80113828
    li r0, 0x3e8
    stw r0, lbl_8047A31C(r13)
    b @L_800202FC
    @L_800202F8:
    li r31, 0x0
    @L_800202FC:
    cmpwi r31, 0x0
    bne @L_8001FE00
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
