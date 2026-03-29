    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r28, r3
    mr r30, r4
    li r31, 0x0
    li r27, 0x0
    b @L_800249F4
    @L_80024720:
    lwz r3, lbl_80478DE4(r13)
    lwzx r3, r3, r27
    cmplwi r3, 0x0
    beq @L_80024740
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800249EC
    @L_80024740:
    lwz r0, lbl_8047A370(r13)
    cmpwi r0, 0x1
    beq @L_80024750
    b @L_800248F0
    @L_80024750:
    lfs f1, lbl_80478898(r13)
    lfs f0, lbl_8047B8D8(r2)
    fcmpo cr0, f1, f0
    ble @L_80024828
    lwz r29, lbl_8047A368(r13)
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lha r3, 0x4(r3)
    bl fn_8005D934
    li r28, 0x0
    @L_80024778:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 24
    cmplwi r0, 0x0
    beq @L_8002479C
    cmpw r29, r28
    bne @L_80024798
    mr r28, r3
    b @L_800247BC
    @L_80024798:
    addi r28, r28, 0x1
    @L_8002479C:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 25
    cmplwi r0, 0x0
    bne @L_800247B8
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_80024778
    @L_800247B8:
    li r28, 0x0
    @L_800247BC:
    li r29, 0x0
    mr r27, r29
    b @L_800247EC
    @L_800247C8:
    lwz r3, lbl_80478DDC(r13)
    addi r0, r27, 0x8
    lwzx r3, r3, r0
    bl fn_8005D934
    cmplw r28, r3
    bne @L_800247E4
    b @L_80024800
    @L_800247E4:
    addi r27, r27, 0x10
    addi r29, r29, 0x1
    @L_800247EC:
    lwz r3, lbl_80478DD8(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    blt @L_800247C8
    li r29, 0x0
    @L_80024800:
    lfs f0, lbl_80478898(r13)
    lfs f1, lbl_8047B8D8(r2)
    lfs f2, lbl_8047B8DC(r2)
    fsubs f0, f0, f1
    fdivs f0, f0, f1
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    b @L_80024994
    @L_80024828:
    lwz r29, lbl_8047A36C(r13)
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lha r3, 0x4(r3)
    bl fn_8005D934
    li r28, 0x0
    @L_80024840:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 24
    cmplwi r0, 0x0
    beq @L_80024864
    cmpw r29, r28
    bne @L_80024860
    mr r28, r3
    b @L_80024884
    @L_80024860:
    addi r28, r28, 0x1
    @L_80024864:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 25
    cmplwi r0, 0x0
    bne @L_80024880
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_80024840
    @L_80024880:
    li r28, 0x0
    @L_80024884:
    li r29, 0x0
    mr r27, r29
    b @L_800248B4
    @L_80024890:
    lwz r3, lbl_80478DDC(r13)
    addi r0, r27, 0x8
    lwzx r3, r3, r0
    bl fn_8005D934
    cmplw r28, r3
    bne @L_800248AC
    b @L_800248C8
    @L_800248AC:
    addi r27, r27, 0x10
    addi r29, r29, 0x1
    @L_800248B4:
    lwz r3, lbl_80478DD8(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    blt @L_80024890
    li r29, 0x0
    @L_800248C8:
    lfs f1, lbl_8047B8D8(r2)
    lfs f0, lbl_80478898(r13)
    lfs f2, lbl_8047B8DC(r2)
    fsubs f0, f1, f0
    fdivs f0, f0, f1
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    b @L_80024994
    @L_800248F0:
    lwz r29, lbl_8047A368(r13)
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lha r3, 0x4(r3)
    bl fn_8005D934
    li r28, 0x0
    @L_80024908:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 24
    cmplwi r0, 0x0
    beq @L_8002492C
    cmpw r29, r28
    bne @L_80024928
    mr r28, r3
    b @L_8002494C
    @L_80024928:
    addi r28, r28, 0x1
    @L_8002492C:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 25
    cmplwi r0, 0x0
    bne @L_80024948
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_80024908
    @L_80024948:
    li r28, 0x0
    @L_8002494C:
    li r29, 0x0
    mr r27, r29
    b @L_8002497C
    @L_80024958:
    lwz r3, lbl_80478DDC(r13)
    addi r0, r27, 0x8
    lwzx r3, r3, r0
    bl fn_8005D934
    cmplw r28, r3
    bne @L_80024974
    b @L_80024990
    @L_80024974:
    addi r27, r27, 0x10
    addi r29, r29, 0x1
    @L_8002497C:
    lwz r3, lbl_80478DD8(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    blt @L_80024958
    li r29, 0x0
    @L_80024990:
    li r0, 0xff
    @L_80024994:
    stb r0, 0x67(r30)
    lwz r3, lbl_80478DD8(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    bge @L_80024A04
    cmplwi r29, 0x9
    ble @L_800249B4
    li r29, 0x0
    @L_800249B4:
    lis r3, lbl_802E4F58@ha
    lwz r5, lbl_80478DE4(r13)
    mulli r4, r31, 0x28
    addi r3, r3, lbl_802E4F58@l
    lbzx r0, r3, r29
    add r3, r5, r4
    slwi r0, r0, 2
    add r3, r3, r0
    lwz r3, 0x4(r3)
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    b @L_80024A18
    @L_800249EC:
    addi r27, r27, 0x28
    addi r31, r31, 0x1
    @L_800249F4:
    lwz r3, lbl_80478DE0(r13)
    lwz r0, 0x0(r3)
    cmplw r31, r0
    blt @L_80024720
    @L_80024A04:
    li r3, 0x1
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    @L_80024A18:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
