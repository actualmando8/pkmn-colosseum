    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0x60
    stmw r23, 0x2c(r1)
    mr r24, r5
    mr r30, r3
    mr r23, r6
    blt @L_801F54D4
    li r3, 0x0
    b @L_801F61A8
@L_801F54D4:
    cmplwi r0, 0x0
    bne @L_801F54E4
    bl fn_801F6B48
    b @L_801F61A8
@L_801F54E4:
    cmplwi r0, 0x8
    bge @L_801F5504
    mr r3, r4
    bl fn_801F6738
    mr. r30, r3
    bne @L_801F5528
    li r3, 0x0
    b @L_801F61A8
@L_801F5504:
    cmplwi r0, 0x5f
    bge @L_801F5528
    cmplwi r30, 0x0
    bne @L_801F5528
    bl fn_801F6B48
    mr. r30, r3
    bne @L_801F5528
    li r3, 0x0
    b @L_801F61A8
@L_801F5528:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x5d
    bgt @L_801F61A4
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, 21676
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r30
    bl fn_801F6600
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6720
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    rlwinm r4, r23, 0, 24, 31
    bl fn_801F66EC
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    bl fn_801F66D4
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    bl fn_801F66BC
    b @L_801F61A8
    mr r3, r30
    bl fn_801F66A4
    b @L_801F61A8
    mr r3, r30
    bl fn_801F668C
    b @L_801F61A8
    mr r3, r30
    mr r4, r23
    bl fn_801F6B18
    b @L_801F61A8
    rlwinm r23, r23, 0, 16, 31
    mr r3, r23
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_801F55E0
    li r3, 0x0
    b @L_801F55EC
@L_801F55E0:
    mr r3, r30
    mr r4, r23
    bl fn_8011B67C
@L_801F55EC:
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    rlwinm r23, r23, 0, 16, 31
    mr r3, r23
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_801F5614
    li r3, 0x0
    b @L_801F5620
@L_801F5614:
    mr r3, r30
    mr r4, r23
    bl fn_8011B444
@L_801F5620:
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6B00
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6AE8
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E0F8
    bl fn_8020E0B0
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0xe
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0xe
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020DD80
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020DD44
    b @L_801F61A8
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801F579C
    li r0, 0x0
    li r4, 0xd
    lis r3, -32737
    stw r4, 0x18(r1)
    addi r4, r3, 13944
    addi r5, r1, 0x18
    stw r0, 0x1c(r1)
    mr r3, r30
    li r6, 0x0
    stw r0, 0x20(r1)
    stw r0, 0x24(r1)
    bl fn_801F37B0
    lwz r0, 0x1c(r1)
    rlwinm. r0, r0, 0, 16, 31
    beq @L_801F5758
    li r0, 0x0
    b @L_801F58C0
@L_801F5758:
    li r0, 0x0
    li r4, 0x4d
    lis r3, -32737
    stw r4, 0x8(r1)
    addi r4, r3, 13944
    addi r5, r1, 0x8
    stw r0, 0xc(r1)
    mr r3, r30
    li r6, 0x0
    stw r0, 0x10(r1)
    stw r0, 0x14(r1)
    bl fn_801F37B0
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 16, 31
    beq @L_801F579C
    li r0, 0x0
    b @L_801F58C0
@L_801F579C:
    mr r3, r30
    li r4, 0x0
    li r5, 0xa
    li r6, 0x4e
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F57C0
    li r0, 0x0
    b @L_801F58C0
@L_801F57C0:
    mr r3, r30
    li r4, 0x0
    li r5, 0xa
    li r6, 0x4f
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F57E4
    li r0, 0x1
    b @L_801F58C0
@L_801F57E4:
    mr r3, r30
    li r4, 0x0
    li r5, 0xa
    li r6, 0x50
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F5808
    li r0, 0x2
    b @L_801F58C0
@L_801F5808:
    mr r3, r30
    li r4, 0x0
    li r5, 0xa
    li r6, 0x51
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F582C
    li r0, 0x3
    b @L_801F58C0
@L_801F582C:
    mr r3, r30
    li r4, 0x0
    li r5, 0xa
    li r6, 0x52
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F5850
    li r0, 0x4
    b @L_801F58C0
@L_801F5850:
    mr r3, r30
    li r4, 0x0
    li r5, 0xa
    li r6, 0x53
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F5874
    li r0, 0x1
    b @L_801F58C0
@L_801F5874:
    mr r3, r30
    li r4, 0x0
    li r5, 0xa
    li r6, 0x54
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F5898
    li r0, 0x2
    b @L_801F58C0
@L_801F5898:
    mr r3, r30
    li r4, 0x0
    li r5, 0xa
    li r6, 0x55
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F58BC
    li r0, 0x3
    b @L_801F58C0
@L_801F58BC:
    li r0, 0x0
@L_801F58C0:
    rlwinm r3, r0, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E0F8
    bl fn_8020E0C8
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E204
    bl fn_8020E1EC
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E204
    bl fn_8020E1D4
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E204
    bl fn_8020E1BC
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E204
    bl fn_8020E1A4
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E124
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E0F8
    bl fn_8020E0E0
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E248
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E4B4
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E470
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E458
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E440
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E428
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E410
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E3F8
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E3E0
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E3C8
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E3B0
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E398
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E380
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E368
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E350
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E338
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E320
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E308
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E2F0
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E2D8
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E2C0
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E2A8
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E290
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E278
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E260
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x1a
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E488
    bl fn_8020E230
    rlwinm r3, r3, 0, 24, 31
    b @L_801F61A8
    mr r3, r30
    mr r4, r23
    bl fn_801F6AB4
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    li r4, 0x0
    li r5, 0xda
    li r6, 0x0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    li r4, 0x0
    li r5, 0xdb
    li r6, 0x0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    li r4, 0x0
    li r5, 0xdc
    li r6, 0x0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    li r4, 0x0
    li r5, 0xdd
    li r6, 0x0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    mr r6, r23
    li r4, 0x0
    li r5, 0xde
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    mr r6, r23
    li r4, 0x0
    li r5, 0xdf
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    mr r6, r23
    li r4, 0x0
    li r5, 0xe0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    li r4, 0x0
    li r5, 0xe1
    li r6, 0x0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    li r4, 0x0
    li r5, 0xe2
    li r6, 0x0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    li r4, 0x0
    li r5, 0xe3
    li r6, 0x0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A98
    li r4, 0x0
    li r5, 0xe4
    li r6, 0x0
    bl fn_8012640C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A7C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A60
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A44
    b @L_801F61A8
    mr r3, r30
    bl fn_801F64C0
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6490
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6460
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6430
    b @L_801F61A8
    bl fn_801F6414
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A28
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6A0C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F69F0
    b @L_801F61A8
    mr r3, r30
    bl fn_801F69D4
    b @L_801F61A8
    mr r3, r30
    bl fn_801F69B8
    b @L_801F61A8
    mr r3, r30
    bl fn_801F699C
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6980
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6964
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6948
    b @L_801F61A8
    mr r3, r30
    bl fn_801F692C
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6910
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    bl fn_801F68F4
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    bl fn_801F68D8
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    bl fn_801F65D4
    extsh r3, r3
    b @L_801F61A8
    mr r3, r30
    rlwinm r4, r23, 0, 16, 31
    bl fn_801F6588
    b @L_801F61A8
    mr r3, r30
    bl fn_801F6544
    b @L_801F61A8
    mr r3, r30
    bl fn_801F64F0
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    bl fn_801EF634
    rlwinm r3, r3, 0, 16, 31
    b @L_801F61A8
    mr r3, r30
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r30
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    li r29, 0x0
    rlwinm r28, r3, 0, 16, 31
    mr r25, r29
    rlwinm r31, r23, 0, 16, 31
    b @L_801F6190
@L_801F60D8:
    li r24, 0x0
    b @L_801F6180
@L_801F60E0:
    li r26, 0x0
    b @L_801F6170
@L_801F60E8:
    mr r3, r30
    mr r6, r26
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r23, r3
    bl fn_801F7404
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801F6110
    li r23, 0x0
@L_801F6110:
    cmplwi r23, 0x0
    bne @L_801F6120
    li r3, 0x0
    b @L_801F6150
@L_801F6120:
    mr r3, r23
    mr r4, r24
    bl fn_801F7258
    cmplwi r3, 0x0
    bne @L_801F613C
    li r3, 0x0
    b @L_801F6150
@L_801F613C:
    mr r4, r25
    bl fn_801F981C
    cmplwi r3, 0x0
    bne @L_801F6150
    li r3, 0x0
@L_801F6150:
    cmplwi r3, 0x0
    beq @L_801F6168
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r31
    bne @L_801F6168
    b @L_801F61A8
@L_801F6168:
    addi r29, r29, 0x1
    addi r26, r26, 0x1
@L_801F6170:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_801F60E8
    addi r24, r24, 0x1
@L_801F6180:
    rlwinm r0, r24, 0, 16, 31
    cmplw r0, r27
    blt @L_801F60E0
    addi r25, r25, 0x1
@L_801F6190:
    rlwinm r0, r25, 0, 16, 31
    cmplw r0, r28
    blt @L_801F60D8
    li r3, 0x0
    b @L_801F61A8
@L_801F61A4:
    li r3, 0x0
@L_801F61A8:
    lmw r23, 0x2c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
