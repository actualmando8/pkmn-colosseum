    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    lha r0, 0x6(r31)
    cmpwi r0, 0xf03
    beq @L_8001F848
    bge @L_8001F37C
    cmpwi r0, 0xefd
    beq @L_8001F5A8
    bge @L_8001F358
    cmpwi r0, 0xefa
    beq @L_8001F3D0
    bge @L_8001F34C
    cmpwi r0, 0x2c4
    beq @L_8001F48C
    b @L_8001FD18
    @L_8001F34C:
    cmpwi r0, 0xefc
    bge @L_8001F538
    b @L_8001F4C8
    @L_8001F358:
    cmpwi r0, 0xf00
    beq @L_8001F6F8
    bge @L_8001F370
    cmpwi r0, 0xeff
    bge @L_8001F688
    b @L_8001F618
    @L_8001F370:
    cmpwi r0, 0xf02
    bge @L_8001F7D8
    b @L_8001F768
    @L_8001F37C:
    cmpwi r0, 0xf09
    beq @L_8001FAE8
    bge @L_8001F3AC
    cmpwi r0, 0xf06
    beq @L_8001F998
    bge @L_8001F3A0
    cmpwi r0, 0xf05
    bge @L_8001F928
    b @L_8001F8B8
    @L_8001F3A0:
    cmpwi r0, 0xf08
    bge @L_8001FA78
    b @L_8001FA08
    @L_8001F3AC:
    cmpwi r0, 0xf0c
    beq @L_8001FC38
    bge @L_8001F3C4
    cmpwi r0, 0xf0b
    bge @L_8001FBC8
    b @L_8001FB58
    @L_8001F3C4:
    cmpwi r0, 0xf0e
    bge @L_8001FD18
    b @L_8001FCA8
    @L_8001F3D0:
    lwz r0, lbl_8047A31C(r13)
    cmpwi r0, 0x1e
    blt @L_8001F3E4
    cmpwi r0, 0x20
    ble @L_8001F3FC
    @L_8001F3E4:
    lbz r0, 0x4(r31)
    li r3, 0x0
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001F410
    @L_8001F3FC:
    lbz r0, 0x4(r31)
    li r3, 0x1
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    @L_8001F410:
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001FD34
    lwz r0, lbl_8047A31C(r13)
    lfs f2, lbl_8047B838(r2)
    cmpwi r0, 0x1f
    lfs f1, lbl_8047A338(r13)
    bne @L_8001F438
    lfs f0, lbl_8047B81C(r2)
    b @L_8001F43C
    @L_8001F438:
    lfs f0, lbl_8047B83C(r2)
    @L_8001F43C:
    fmuls f0, f1, f0
    fmuls f1, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f2, lbl_8047B844(r2)
    lfs f1, lbl_8047B840(r2)
    lfs f0, lbl_8047B848(r2)
    fmadds f1, f2, f3, f1
    fcmpo cr0, f1, f0
    ble @L_8001F468
    fmr f1, f0
    @L_8001F468:
    lfs f0, lbl_8047B814(r2)
    fcmpo cr0, f1, f0
    bge @L_8001F478
    fmr f1, f0
    @L_8001F478:
    fctiwz f0, f1
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x67(r31)
    b @L_8001FD34
    @L_8001F48C:
    lwz r0, lbl_8047A31C(r13)
    cmpwi r0, 0x1e
    blt @L_8001F4A0
    cmpwi r0, 0x20
    ble @L_8001F4B4
    @L_8001F4A0:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F4B4:
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F4C8:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x0
    bne @L_8001F524
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x1c(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F524:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F538:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x0
    bne @L_8001F594
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x20(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F594:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F5A8:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x0
    bne @L_8001F604
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x10(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F604:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F618:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x0
    bne @L_8001F674
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x14(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F674:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F688:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x1
    bne @L_8001F6E4
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x1c(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F6E4:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F6F8:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x1
    bne @L_8001F754
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x20(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F754:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F768:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x1
    bne @L_8001F7C4
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x24(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F7C4:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F7D8:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x1
    bne @L_8001F834
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x10(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F834:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F848:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x1
    bne @L_8001F8A4
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x14(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F8A4:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F8B8:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x1
    bne @L_8001F914
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x18(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F914:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F928:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x2
    bne @L_8001F984
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x1c(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F984:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001F998:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x2
    bne @L_8001F9F4
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x20(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001F9F4:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001FA08:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x2
    bne @L_8001FA64
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x10(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001FA64:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001FA78:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x2
    bne @L_8001FAD4
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x14(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001FAD4:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001FAE8:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x2
    bne @L_8001FB44
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x18(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001FB44:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001FB58:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x3
    bne @L_8001FBB4
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x10(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001FBB4:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001FBC8:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x3
    bne @L_8001FC24
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x14(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001FC24:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001FC38:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x3
    bne @L_8001FC94
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x1c(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001FC94:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001FCA8:
    lwz r0, lbl_80478878(r13)
    cmpwi r0, 0x3
    bne @L_8001FD04
    lbz r0, 0x4(r31)
    lis r3, lbl_803A1F88@ha
    lis r4, lbl_802EF0A8@ha
    ori r0, r0, 0x2
    addi r3, r3, lbl_803A1F88@l
    extsb r5, r0
    addi r0, r4, lbl_802EF0A8@l
    stb r5, 0x4(r31)
    lfs f0, 0x20(r3)
    lha r3, 0x6(r31)
    fctiwz f0, f0
    mulli r3, r3, 0x1c
    stfd f0, 0x8(r1)
    add r3, r0, r3
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    b @L_8001FD34
    @L_8001FD04:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8001FD34
    @L_8001FD18:
    lbz r0, lbl_8047A34C(r13)
    cmplwi r0, 0x0
    bne @L_8001FD34
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    @L_8001FD34:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
