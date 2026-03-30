    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r29, r3
    mr r30, r4
    lis r3, -32704
    addi r3, r3, -19584
    lwz r28, 0xc(r3)
    cmplwi r28, 0x0
    beq @L_80096C28
    mr r3, r28
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_80096C28
    lha r0, 0x6(r30)
    li r4, -0x100
    lbz r5, 0x8b(r29)
    lbz r6, 0x95(r29)
    cmpwi r0, 0x10f
    or r31, r5, r4
    extsb r6, r6
    beq @L_80096C28
    bge @L_800966A0
    cmpwi r0, 0x106
    beq @L_80096B14
    bge @L_8009667C
    cmpwi r0, 0xf3
    beq @L_80096AC4
    bge @L_8009666C
    cmpwi r0, 0xe7
    beq @L_800966F0
    b @L_80096C28
@L_8009666C:
    cmpwi r0, 0xf5
    beq @L_80096ADC
    bge @L_80096C28
    b @L_80096AF8
@L_8009667C:
    cmpwi r0, 0x10c
    beq @L_80096864
    bge @L_80096694
    cmpwi r0, 0x10b
    bge @L_80096828
    b @L_800967B0
@L_80096694:
    cmpwi r0, 0x10e
    bge @L_800968E4
    b @L_800968BC
@L_800966A0:
    cmpwi r0, 0x553
    beq @L_80096C28
    bge @L_800966D4
    cmpwi r0, 0x112
    beq @L_80096ADC
    bge @L_800966C4
    cmpwi r0, 0x111
    bge @L_80096AF8
    b @L_80096AC4
@L_800966C4:
    cmpwi r0, 0x551
    beq @L_80096918
    bge @L_80096954
    b @L_80096C28
@L_800966D4:
    cmpwi r0, 0x598
    beq @L_80096B94
    bge @L_80096C28
    cmpwi r0, 0x555
    beq @L_80096A08
    bge @L_80096C28
    b @L_800969A8
@L_800966F0:
    lis r3, -32704
    addi r3, r3, -19656
    bl fn_80109934
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_80096C28
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, -32719
    addi r3, r3, 20376
    bl fn_800D7820
    mr r4, r28
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x2
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, -29808(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lha r3, 0x54(r30)
    lha r4, 0x56(r30)
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, -29848(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    b @L_80096C28
@L_800967B0:
    mr r3, r28
    li r28, 0x0
    li r4, 0x0
    li r5, 0xbb
    li r6, 0x0
    bl fn_8012640C
    lha r0, 0x6(r30)
    rlwinm r3, r3, 0, 24, 31
    cmpwi r0, 0x109
    beq @L_80096808
    bge @L_800967EC
    cmpwi r0, 0x107
    beq @L_800967F8
    bge @L_80096800
    b @L_80096814
@L_800967EC:
    cmpwi r0, 0x10b
    bge @L_80096814
    b @L_80096810
@L_800967F8:
    li r28, 0x8
    b @L_80096814
@L_80096800:
    li r28, 0x4
    b @L_80096814
@L_80096808:
    li r28, 0x2
    b @L_80096814
@L_80096810:
    li r28, 0x1
@L_80096814:
    and r0, r3, r28
    mr r3, r30
    rlwinm r4, r0, 0, 24, 31
    bl fn_80109220
    b @L_80096C28
@L_80096828:
    mr r3, r28
    bl fn_8011F550
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xd
    bge @L_80096C28
    lis r3, -32721
    rlwinm r0, r0, 1, 0, 30
    addi r3, r3, -4824
    mr r5, r29
    lhzx r6, r3, r0
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80096C28
@L_80096864:
    mr r3, r28
    li r4, 0x0
    li r5, 0xb5
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 24, 31
    rlwinm r0, r3, 0, 28, 31
    cmpwi r0, 0x0
    beq @L_80096890
    li r6, 0xe8
    b @L_800968A4
@L_80096890:
    cmplwi r3, 0x0
    beq @L_800968A0
    li r6, 0xe7
    b @L_800968A4
@L_800968A0:
    li r6, 0x0
@L_800968A4:
    mr r5, r29
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80096C28
@L_800968BC:
    mr r3, r28
    li r4, 0x1
    bl fn_8001D624
    mr r6, r3
    mr r5, r29
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80096C28
@L_800968E4:
    mr r3, r28
    bl fn_801230E0
    mr r0, r3
    mr r3, r30
    rlwinm r0, r0, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_80096908
    li r0, 0x1
    b @L_8009690C
@L_80096908:
    li r0, 0x0
@L_8009690C:
    rlwinm r4, r0, 0, 24, 31
    bl fn_80109220
    b @L_80096C28
@L_80096918:
    mr r3, r28
    bl fn_801230E0
    rlwinm r4, r3, 0, 16, 31
    cmplwi r4, 0x0
    beq @L_80096C28
    li r3, 0x2d
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r31
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2bd3
    bl fn_800FBB34
    b @L_80096C28
@L_80096954:
    lwz r3, 0x4c(r30)
    bl fn_800FA444
    rlwinm r0, r3, 16, 16, 31
    mr r3, r28
    extsh r0, r0
    li r4, 0x0
    mr r28, r0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r3, r28
    lha r6, 0x56(r30)
    mr r7, r31
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FBB34
    b @L_80096C28
@L_800969A8:
    bl fn_8011E760
    mr r29, r3
    li r3, 0x2bd4
    bl fn_800FA444
    rlwinm r0, r3, 16, 16, 31
    mr r5, r31
    extsh r30, r0
    li r3, 0x0
    li r4, 0x0
    li r6, 0x2bd4
    bl fn_800FB680
    cmplwi r29, 0x0
    beq @L_80096C28
    mr r3, r29
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    mr r3, r30
    mr r5, r31
    li r4, 0x0
    li r6, 0xe7
    bl fn_800FB680
    b @L_80096C28
@L_80096A08:
    mr r3, r28
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r31
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xe7
    bl fn_800FBB34
    li r3, 0xe7
    bl fn_800FA444
    rlwinm r0, r3, 16, 16, 31
    mr r3, r28
    extsh r29, r0
    bl fn_8001DA60
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x1
    beq @L_80096A88
    bge @L_80096A78
    cmpwi r0, 0x0
    bge @L_80096A80
    b @L_80096A90
@L_80096A78:
    cmpwi r0, 0x3
    b @L_80096A90
@L_80096A80:
    li r3, 0xd67
    b @L_80096A94
@L_80096A88:
    li r3, 0xd68
    b @L_80096A94
@L_80096A90:
    li r3, 0x0
@L_80096A94:
    cmplwi r3, 0x0
    beq @L_80096C28
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    mr r3, r29
    mr r5, r31
    li r4, 0x0
    li r6, 0xcf
    bl fn_800FB680
    b @L_80096C28
@L_80096AC4:
    cntlzw r0, r6
    mr r3, r30
    rlwinm r0, r0, 27, 5, 31
    rlwinm r4, r0, 0, 24, 31
    bl fn_80109220
    b @L_80096C28
@L_80096ADC:
    subfic r0, r6, 0x1
    mr r3, r30
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r4, r0, 0, 24, 31
    bl fn_80109220
    b @L_80096C28
@L_80096AF8:
    subfic r0, r6, 0x2
    mr r3, r30
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r4, r0, 0, 24, 31
    bl fn_80109220
    b @L_80096C28
@L_80096B14:
    lis r3, -32704
    li r4, 0x0
    addi r3, r3, -19584
    lbz r0, 0x1(r3)
    cmpwi r0, 0x4
    beq @L_80096B58
    bge @L_80096B44
    cmpwi r0, 0x2
    beq @L_80096B58
    bge @L_80096B60
    cmpwi r0, 0x1
    b @L_80096B88
@L_80096B44:
    cmpwi r0, 0x8
    bge @L_80096B88
    cmpwi r0, 0x6
    bge @L_80096B88
    b @L_80096B78
@L_80096B58:
    li r4, 0x1
    b @L_80096B88
@L_80096B60:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80096B88
    li r4, 0x1
    b @L_80096B88
@L_80096B78:
    lwz r0, 0x1c(r3)
    cmpwi r0, 0x0
    ble @L_80096B88
    li r4, 0x1
@L_80096B88:
    mr r3, r30
    bl fn_80109220
    b @L_80096C28
@L_80096B94:
    lis r3, -32704
    li r6, 0x0
    addi r3, r3, -19584
    lbz r0, 0x1(r3)
    cmpwi r0, 0x4
    beq @L_80096C0C
    bge @L_80096BC4
    cmpwi r0, 0x2
    beq @L_80096BD8
    bge @L_80096BF4
    cmpwi r0, 0x1
    b @L_80096C10
@L_80096BC4:
    cmpwi r0, 0x8
    bge @L_80096C10
    cmpwi r0, 0x6
    bge @L_80096C10
    b @L_80096BE0
@L_80096BD8:
    li r6, 0x2bcf
    b @L_80096C10
@L_80096BE0:
    lwz r0, 0x1c(r3)
    cmpwi r0, 0x0
    ble @L_80096C10
    li r6, 0x2bd2
    b @L_80096C10
@L_80096BF4:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80096C10
    li r6, 0x2bd0
    b @L_80096C10
@L_80096C0C:
    li r6, 0x2bd0
@L_80096C10:
    cmplwi r6, 0x0
    beq @L_80096C28
    mr r5, r31
    li r3, 0x0
    li r4, 0x0
    bl fn_800FB680
@L_80096C28:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
