    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    stw r28, 0x30(r1)
    mr r29, r4
    lis r4, lbl_80271E00@ha
    rlwinm r0, r3, 2, 0, 29
    addi r4, r4, lbl_80271E00@l
    addi r3, r1, 0x8
    lwz r7, 0x0(r4)
    li r31, 0x0
    lwz r6, 0x4(r4)
    lwz r5, 0x8(r4)
    lwz r4, 0xc(r4)
    stw r7, 0x8(r1)
    stw r6, 0xc(r1)
    stw r5, 0x10(r1)
    stw r4, 0x14(r1)
    lwzx r30, r3, r0
    mr r3, r30
    bl fn_800F7EF8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80103688
    li r3, 0x0
    b @L_801038D8
@L_80103688:
    mr r3, r30
    li r4, 0x0
    bl fn_800F7A08
    mr r28, r3
    mr r3, r30
    li r4, 0x0
    bl fn_800F7A7C
    extsb r0, r3
    cmpwi r0, 0x0
    bge @L_801036B4
    neg r0, r0
@L_801036B4:
    cmpwi r0, 0x20
    bgt @L_801036D4
    extsb r0, r28
    cmpwi r0, 0x0
    bge @L_801036CC
    neg r0, r0
@L_801036CC:
    cmpwi r0, 0x20
    ble @L_801037D8
@L_801036D4:
    extsb r4, r3
    extsb r0, r28
    lis r3, 0x4330
    lfd f2, lbl_8047CDD8(r13)
    xoris r4, r4, 0x8000
    xoris r0, r0, 0x8000
    stw r4, 0x1c(r1)
    stw r3, 0x18(r1)
    lfd f0, 0x18(r1)
    stw r0, 0x24(r1)
    fsub f1, f0, f2
    stw r3, 0x20(r1)
    lfd f0, 0x20(r1)
    fsub f2, f0, f2
    bl fn_800CE2D8
    frsp f2, f1
    lfs f0, lbl_8047CDC0(r13)
    fcmpo cr0, f2, f0
    ble @L_80103728
    fmr f1, f2
    b @L_8010372C
@L_80103728:
    fneg f1, f2
@L_8010372C:
    lfs f0, lbl_8047CDC8(r13)
    fcmpo cr0, f1, f0
    bge @L_80103744
    ori r0, r31, 0x2
    rlwinm r31, r0, 0, 16, 31
    b @L_80103770
@L_80103744:
    lfs f0, lbl_8047CDC0(r13)
    fcmpo cr0, f2, f0
    ble @L_80103758
    fmr f1, f2
    b @L_8010375C
@L_80103758:
    fneg f1, f2
@L_8010375C:
    lfs f0, lbl_8047CDCC(r13)
    fcmpo cr0, f1, f0
    ble @L_80103770
    ori r0, r31, 0x1
    rlwinm r31, r0, 0, 16, 31
@L_80103770:
    lfs f0, lbl_8047CDC0(r13)
    lfs f1, lbl_8047CDD0(r13)
    fcmpo cr0, f2, f0
    ble @L_80103788
    fmr f0, f2
    b @L_8010378C
@L_80103788:
    fneg f0, f2
@L_8010378C:
    fcmpo cr0, f1, f0
    bge @L_801037D8
    lfs f0, lbl_8047CDC0(r13)
    fcmpo cr0, f2, f0
    ble @L_801037A8
    fmr f1, f2
    b @L_801037AC
@L_801037A8:
    fneg f1, f2
@L_801037AC:
    lfs f0, lbl_8047CDD4(r13)
    fcmpo cr0, f1, f0
    bge @L_801037D8
    lfs f0, lbl_8047CDC0(r13)
    fcmpo cr0, f2, f0
    bge @L_801037D0
    ori r0, r31, 0x4
    rlwinm r31, r0, 0, 16, 31
    b @L_801037D8
@L_801037D0:
    ori r0, r31, 0x8
    rlwinm r31, r0, 0, 16, 31
@L_801037D8:
    mr r3, r30
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_801037F4
    ori r0, r31, 0x1
    rlwinm r31, r0, 0, 16, 31
@L_801037F4:
    rlwinm r0, r3, 0, 29, 29
    cmplwi r0, 0x0
    beq @L_80103808
    ori r0, r31, 0x2
    rlwinm r31, r0, 0, 16, 31
@L_80103808:
    rlwinm r0, r3, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8010381C
    ori r0, r31, 0x4
    rlwinm r31, r0, 0, 16, 31
@L_8010381C:
    rlwinm r0, r3, 0, 30, 30
    cmplwi r0, 0x0
    beq @L_80103830
    ori r0, r31, 0x8
    rlwinm r31, r0, 0, 16, 31
@L_80103830:
    rlwinm r0, r3, 0, 23, 23
    cmplwi r0, 0x0
    beq @L_80103844
    ori r0, r31, 0x10
    rlwinm r31, r0, 0, 16, 31
@L_80103844:
    rlwinm r0, r3, 0, 22, 22
    cmplwi r0, 0x0
    beq @L_80103858
    ori r0, r31, 0x20
    rlwinm r31, r0, 0, 16, 31
@L_80103858:
    rlwinm r0, r3, 0, 21, 21
    cmplwi r0, 0x0
    beq @L_8010386C
    ori r0, r31, 0x40
    rlwinm r31, r0, 0, 16, 31
@L_8010386C:
    rlwinm r0, r3, 0, 20, 20
    cmplwi r0, 0x0
    beq @L_80103880
    ori r0, r31, 0x80
    rlwinm r31, r0, 0, 16, 31
@L_80103880:
    rlwinm r0, r3, 0, 27, 27
    cmplwi r0, 0x0
    beq @L_80103894
    ori r0, r31, 0x100
    rlwinm r31, r0, 0, 16, 31
@L_80103894:
    rlwinm r0, r3, 0, 25, 25
    cmplwi r0, 0x0
    beq @L_801038A8
    ori r0, r31, 0x200
    rlwinm r31, r0, 0, 16, 31
@L_801038A8:
    rlwinm r0, r3, 0, 26, 26
    cmplwi r0, 0x0
    beq @L_801038BC
    ori r0, r31, 0x400
    rlwinm r31, r0, 0, 16, 31
@L_801038BC:
    rlwinm r0, r3, 0, 19, 19
    cmplwi r0, 0x0
    beq @L_801038D0
    ori r0, r31, 0x800
    rlwinm r31, r0, 0, 16, 31
@L_801038D0:
    sth r31, 0x0(r29)
    li r3, 0x1
@L_801038D8:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r28, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
