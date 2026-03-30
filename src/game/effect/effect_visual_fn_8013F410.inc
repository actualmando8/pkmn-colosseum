    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    stw r29, 0x44(r1)
    stw r28, 0x40(r1)
    mr. r28, r3
    beq @L_8013F7E8
    lwz r0, 0x4(r28)
    cmplwi r0, 0x0
    beq @L_8013F44C
    lwz r0, lbl_8047AEE8(r13)
    cmplwi r0, 0x0
    bne @L_8013F454
    @L_8013F44C:
    li r3, 0x0
    b @L_8013F7EC
    @L_8013F454:
    bl fn_8013F97C
    lwz r0, lbl_8047AEE8(r13)
    cmplwi r0, 0x0
    beq @L_8013F4FC
    bl fn_800E3B3C
    mr. r31, r3
    beq @L_8013F4FC
    li r3, 0x2
    bl fn_800D4604
    bl fn_800D2248
    li r3, 0x1
    bl fn_800D377C
    lwz r3, lbl_8047AEE8(r13)
    li r4, 0x0
    bl fn_800D3410
    li r30, 0x0
    b @L_8013F4C4
    @L_8013F498:
    mr r3, r30
    bl fn_800E3B08
    mr. r29, r3
    beq @L_8013F4C0
    bl fn_800E3D08
    clrlwi. r0, r3, 24
    beq @L_8013F4C0
    mr r3, r29
    li r4, 0x3010
    bl fn_800E3760
    @L_8013F4C0:
    addi r30, r30, 0x1
    @L_8013F4C4:
    cmpw r30, r31
    blt @L_8013F498
    lhz r0, 0x28(r28)
    cmplwi r0, 0x0
    beq @L_8013F4F0
    li r3, 0x2000
    li r4, 0x0
    bl fn_80118104
    li r3, 0x2000
    li r4, 0x1
    bl fn_80118104
    @L_8013F4F0:
    bl fn_800D3190
    li r3, 0x1
    bl fn_800D4604
    @L_8013F4FC:
    li r3, 0x0
    li r4, 0x2
    li r5, 0x0
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    lis r3, 0x8000
    addi r3, r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x0
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x1
    bl fn_800D9ED8
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    lwz r4, lbl_8047AEE8(r13)
    li r3, 0x0
    bl fn_800D85D4
    lwz r4, 0x4(r28)
    li r3, 0x1
    bl fn_800D85D4
    addi r6, r28, 0x68
    li r3, 0x0
    li r4, 0x2
    li r5, 0x4
    bl fn_800D848C
    addi r3, r1, 0x8
    bl fn_800E064C
    addi r6, r1, 0x8
    li r3, 0x1
    li r4, 0x2
    li r5, 0x5
    bl fn_800D848C
    li r3, 0x1
    bl fn_800DC1D4
    li r3, 0x1
    bl fn_800DBA54
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DB9F0
    lwz r4, lbl_8047AEF0(r13)
    li r3, 0x0
    mr r5, r4
    bl fn_800DB988
    addi r4, r28, 0x98
    li r3, 0x1
    li r5, 0x1
    bl fn_800DB900
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    li r7, 0x1
    bl fn_800DBCE4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x2
    bl fn_800DC224
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC14C
    li r3, 0x0
    li r4, 0xf
    li r5, 0xf
    li r6, 0xf
    li r7, 0x8
    bl fn_800DC0D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC04C
    li r3, 0x0
    li r4, 0x7
    li r5, 0x7
    li r6, 0x7
    li r7, 0x4
    bl fn_800DBFD4
    lwz r3, 0x0(r28)
    bl fn_800D7820
    li r3, 0x6
    bl fn_800D6A00
    addi r3, r28, 0x38
    bl fn_800D7F14
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, lbl_8047D304(r2)
    lfs f3, lbl_8047D300(r2)
    fmr f2, f1
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5C18
    lfs f1, lbl_8047D300(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, lbl_8047D300(r2)
    li r3, 0x1
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, lbl_8047D308(r2)
    lfs f2, lbl_8047D304(r2)
    lfs f3, lbl_8047D300(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5C18
    lfs f1, lbl_8047D308(r2)
    li r3, 0x0
    lfs f2, lbl_8047D300(r2)
    bl fn_800D59B8
    lfs f1, lbl_8047D308(r2)
    li r3, 0x1
    lfs f2, lbl_8047D300(r2)
    bl fn_800D59B8
    lfs f1, lbl_8047D308(r2)
    lfs f3, lbl_8047D300(r2)
    fmr f2, f1
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5C18
    lfs f1, lbl_8047D308(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, lbl_8047D308(r2)
    li r3, 0x1
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, lbl_8047D304(r2)
    lfs f2, lbl_8047D308(r2)
    lfs f3, lbl_8047D300(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5C18
    lfs f1, lbl_8047D300(r2)
    li r3, 0x0
    lfs f2, lbl_8047D308(r2)
    bl fn_800D59B8
    lfs f1, lbl_8047D300(r2)
    li r3, 0x1
    lfs f2, lbl_8047D308(r2)
    bl fn_800D59B8
    bl fn_800D6728
    bl fn_800D7E5C
    addi r6, r1, 0x8
    li r3, 0x0
    li r4, 0x2
    li r5, 0x4
    bl fn_800D848C
    li r3, 0x0
    bl fn_800DBE5C
    li r3, 0x0
    bl fn_800DBA54
    lis r3, 0x8000
    addi r3, r3, 0x2
    bl fn_800D888C
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x1
    b @L_8013F7EC
    @L_8013F7E8:
    li r3, 0x0
    @L_8013F7EC:
    lwz r0, 0x54(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    lwz r28, 0x40(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
