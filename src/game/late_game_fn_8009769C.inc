    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r5
    lis r3, -32704
    li r4, 0xc8
    addi r3, r3, -19656
    li r5, 0xb4
    bl fn_8010A5BC
    lis r3, -32704
    mr r4, r28
    addi r3, r3, -19656
    bl fn_80109C88
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80097734
    li r3, 0x1
    bl fn_801C40F0
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_80097728
    lfs f1, -29804(r2)
    li r3, 0x2
    bl fn_801C41C8
    b @L_80097734
@L_80097728:
    lfs f1, -29800(r2)
    li r3, 0x2
    bl fn_801C41C8
@L_80097734:
    li r0, 0x0
    lis r3, -32704
    stw r0, 0x8(r1)
    addi r31, r3, -19584
@L_80097744:
    addi r5, r1, 0x8
    li r3, 0x53
    li r4, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_80097774
    stw r3, 0x4(r31)
    b @L_80097818
@L_80097774:
    lbz r0, 0x0(r31)
    lbz r3, 0x2(r31)
    rlwinm r0, r0, 0, 25, 25
    extsb r28, r3
    cmpwi r0, 0x0
    stw r28, 0x4(r31)
    beq @L_80097818
    rlwinm r0, r28, 0, 16, 31
    lwz r29, 0xc(r31)
    cmplwi r0, 0x4
    bne @L_800977A8
    lhz r30, 0x18(r31)
    b @L_800977DC
@L_800977A8:
    mr r3, r29
    mr r6, r28
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
    mr r3, r29
    mr r4, r28
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800977DC
    li r30, 0x0
@L_800977DC:
    mr r4, r30
    li r3, 0x0
    li r5, 0x19
    li r6, 0x0
    bl fn_8011BEB4
    cmpwi r3, 0x0
    beq @L_80097818
    li r3, 0x2
    li r4, 0x2be9
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80097744
@L_80097818:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80097874
    li r3, 0x1
    bl fn_801C40F0
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_80097860
    lfs f1, -29804(r2)
    li r3, 0x3
    bl fn_801C41C8
    b @L_8009786C
@L_80097860:
    lfs f1, -29800(r2)
    li r3, 0x3
    bl fn_801C41C8
@L_8009786C:
    li r3, 0x1
    bl fn_801C40F0
@L_80097874:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r3, 0x0(r3)
    rlwinm r0, r3, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_800978B4
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    bne @L_800978B4
    li r3, 0x1
    bl fn_801C40F0
    lfs f1, -29800(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
@L_800978B4:
    li r3, 0x54
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800978D8
    li r3, 0x54
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
@L_800978D8:
    li r3, 0x55
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800978FC
    li r3, 0x55
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
@L_800978FC:
    li r3, 0x57
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80097920
    li r3, 0x57
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
@L_80097920:
    li r3, 0x56
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80097944
    li r3, 0x56
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
@L_80097944:
    li r3, 0x58
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80097968
    li r3, 0x58
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
@L_80097968:
    li r3, 0x53
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_800979B8
    bl fn_800FF660
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_800979B8
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
@L_800979B8:
    lis r3, -32704
    addi r3, r3, -19656
    bl fn_8010A420
    bl fn_800F0308
    lwz r3, 0x4(r31)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
