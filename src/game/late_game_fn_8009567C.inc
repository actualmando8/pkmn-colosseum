    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r24, 0x20(r1)
    mr r28, r3
    mr r30, r4
    lis r3, -32704
    addi r3, r3, -19584
    lwz r31, 0xc(r3)
    cmplwi r31, 0x0
    beq @L_800965B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    bl fn_8011E778
    mr r26, r3
    cmplwi r26, 0x0
    beq @L_800965B4
    lha r0, 0x6(r30)
    li r25, 0x1
    cmpwi r0, 0x1b8
    bge @L_80095708
    cmpwi r0, 0x18b
    bge @L_800956FC
    cmpwi r0, 0x182
    bge @L_80095790
    cmpwi r0, 0x170
    bge @L_80095724
    b @L_80095790
@L_800956FC:
    cmpwi r0, 0x191
    bge @L_80095790
    b @L_80095724
@L_80095708:
    cmpwi r0, 0x1d3
    bge @L_8009571C
    cmpwi r0, 0x1ca
    bge @L_80095790
    b @L_80095724
@L_8009571C:
    cmpwi r0, 0x1d9
    bge @L_80095790
@L_80095724:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x1(r3)
    cmpwi r0, 0x7
    beq @L_80095750
    bge @L_80095780
    cmpwi r0, 0x5
    bge @L_80095780
    cmpwi r0, 0x3
    bge @L_80095750
    b @L_80095780
@L_80095750:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x2(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80095778
    cmpwi r0, 0x4
    bgt @L_80095778
    li r25, 0x1
    b @L_80095784
@L_80095778:
    li r25, 0x0
    b @L_80095784
@L_80095780:
    li r25, 0x0
@L_80095784:
    mr r3, r30
    mr r4, r25
    bl fn_80109220
@L_80095790:
    rlwinm r0, r25, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800965B4
    lha r4, 0x6(r30)
    li r0, -0x100
    lbz r3, 0x8b(r28)
    cmpwi r4, 0x57b
    or r29, r3, r0
    beq @L_80095910
    bge @L_8009586C
    cmpwi r4, 0x14b
    beq @L_800960B8
    bge @L_8009581C
    cmpwi r4, 0x144
    beq @L_80095F78
    bge @L_800957F8
    cmpwi r4, 0x13c
    beq @L_80095940
    bge @L_800957E8
    cmpwi r4, 0x13b
    bge @L_80095910
    b @L_800965B4
@L_800957E8:
    cmpwi r4, 0x142
    beq @L_80095988
    bge @L_80095EFC
    b @L_800965B4
@L_800957F8:
    cmpwi r4, 0x148
    beq @L_80095FF8
    bge @L_80095810
    cmpwi r4, 0x147
    bge @L_80095FB8
    b @L_800965B4
@L_80095810:
    cmpwi r4, 0x14a
    bge @L_80096078
    b @L_80096038
@L_8009581C:
    cmpwi r4, 0x158
    beq @L_80096330
    bge @L_80095854
    cmpwi r4, 0x154
    beq @L_80096210
    bge @L_80095848
    cmpwi r4, 0x153
    bge @L_80096188
    cmpwi r4, 0x14d
    bge @L_800965B4
    b @L_800960F8
@L_80095848:
    cmpwi r4, 0x156
    bge @L_800965B4
    b @L_800962C0
@L_80095854:
    cmpwi r4, 0x54e
    beq @L_80095BC8
    bge @L_800965B4
    cmpwi r4, 0x54d
    bge @L_800961CC
    b @L_800965B4
@L_8009586C:
    cmpwi r4, 0x58f
    beq @L_80096188
    bge @L_800958CC
    cmpwi r4, 0x584
    beq @L_80095FF8
    bge @L_800958A8
    cmpwi r4, 0x581
    beq @L_80095988
    bge @L_8009589C
    cmpwi r4, 0x57d
    beq @L_80095940
    b @L_800965B4
@L_8009589C:
    cmpwi r4, 0x583
    bge @L_80095FB8
    b @L_80095E84
@L_800958A8:
    cmpwi r4, 0x587
    beq @L_800960B8
    bge @L_800958C0
    cmpwi r4, 0x586
    bge @L_80096078
    b @L_80096038
@L_800958C0:
    cmpwi r4, 0x589
    bge @L_800965B4
    b @L_800960F8
@L_800958CC:
    cmpwi r4, 0x595
    beq @L_800963CC
    bge @L_800958F0
    cmpwi r4, 0x592
    beq @L_800962C0
    bge @L_800965B4
    cmpwi r4, 0x591
    bge @L_80096210
    b @L_800961CC
@L_800958F0:
    cmpwi r4, 0x12b8
    bge @L_80095904
    cmpwi r4, 0x599
    beq @L_80095BC8
    b @L_800965B4
@L_80095904:
    cmpwi r4, 0x12bc
    bge @L_800965B4
    b @L_800963CC
@L_80095910:
    mr r3, r26
    li r4, 0x0
    bl fn_8011E474
    rlwinm r3, r3, 0, 24, 31
    bl fn_8010C46C
    rlwinm r6, r3, 0, 16, 31
    mr r5, r28
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_800965B4
@L_80095940:
    mr r3, r26
    li r4, 0x0
    bl fn_8011E474
    rlwinm r24, r3, 0, 24, 31
    mr r3, r26
    li r4, 0x1
    bl fn_8011E474
    rlwinm r3, r3, 0, 24, 31
    cmplw r24, r3
    beq @L_800965B4
    bl fn_8010C46C
    rlwinm r6, r3, 0, 16, 31
    mr r5, r28
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_800965B4
@L_80095988:
    mr r3, r31
    bl fn_8011F77C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x3
    bge @L_800959A4
    li r24, 0x934
    b @L_800959C8
@L_800959A4:
    mr r3, r31
    li r4, 0x0
    li r5, 0xbf
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 24, 31
    bl fn_8011CE18
    bl fn_8011CE00
    mr r24, r3
@L_800959C8:
    mr r4, r24
    li r3, 0x55
    bl fn_80132A38
    cmpwi r24, 0xc96
    beq @L_800959EC
    bge @L_800959FC
    cmpwi r24, 0xc86
    beq @L_800959EC
    b @L_800959FC
@L_800959EC:
    li r3, 0x56
    li r4, 0x1
    bl fn_80132A38
    b @L_80095A08
@L_800959FC:
    li r3, 0x56
    li r4, 0x2bd8
    bl fn_80132A38
@L_80095A08:
    mr r3, r31
    li r4, 0x0
    li r5, 0x72
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    cmplwi r4, 0x0
    bne @L_80095A2C
    li r4, 0x5
@L_80095A2C:
    li r3, 0x34
    bl fn_80132A38
    lis r3, -32704
    cmplwi r31, 0x0
    addi r3, r3, -19584
    lwz r25, 0x8(r3)
    bne @L_80095A50
    li r0, 0x0
    b @L_80095B60
@L_80095A50:
    mr r3, r31
    li r4, 0x0
    li r5, 0x70
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x2
    bl fn_80135938
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xb
    beq @L_80095A80
    li r0, 0x0
    b @L_80095B60
@L_80095A80:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80095ACC
    cmplwi r25, 0x0
    bne @L_80095AAC
    li r3, 0x0
    bl fn_801F2A7C
    mr r25, r3
@L_80095AAC:
    cmplwi r25, 0x0
    bne @L_80095ABC
    li r0, 0x0
    b @L_80095B60
@L_80095ABC:
    mr r3, r25
    bl fn_801FCEAC
    mr r25, r3
    b @L_80095AF8
@L_80095ACC:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_80095AF0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    mr r25, r3
    b @L_80095AF8
@L_80095AF0:
    bl fn_8006AEEC
    mr r25, r3
@L_80095AF8:
    mr r3, r25
    bl fn_8012AC54
    mr r26, r3
    mr r3, r25
    bl fn_8012AC3C
    mr r25, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x75
    li r6, 0x0
    bl fn_8012640C
    cmplw r25, r3
    bne @L_80095B5C
    mr r3, r31
    li r4, 0x0
    li r5, 0x76
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    mr r3, r26
    bl fn_800F9EE4
    cmpwi r3, 0x0
    bne @L_80095B5C
    li r0, 0x1
    b @L_80095B60
@L_80095B5C:
    li r0, 0x0
@L_80095B60:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80095BA8
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0xc6
    bge @L_80095BA0
    cmpwi r0, 0xc4
    bge @L_80095B98
    b @L_80095BA0
@L_80095B98:
    li r8, 0x2be3
    b @L_80095BAC
@L_80095BA0:
    li r8, 0x2bcd
    b @L_80095BAC
@L_80095BA8:
    li r8, 0x2bcd
@L_80095BAC:
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    bl fn_800FBB34
    b @L_800965B4
@L_80095BC8:
    mr r3, r31
    li r26, 0x2be6
    li r4, 0x0
    li r5, 0x71
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    bl fn_8011396C
    lis r4, -32704
    cmplwi r31, 0x0
    addi r4, r4, -19584
    mr r27, r3
    lwz r24, 0x8(r4)
    bne @L_80095C08
    li r0, 0x0
    b @L_80095D18
@L_80095C08:
    mr r3, r31
    li r4, 0x0
    li r5, 0x70
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x2
    bl fn_80135938
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xb
    beq @L_80095C38
    li r0, 0x0
    b @L_80095D18
@L_80095C38:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80095C84
    cmplwi r24, 0x0
    bne @L_80095C64
    li r3, 0x0
    bl fn_801F2A7C
    mr r24, r3
@L_80095C64:
    cmplwi r24, 0x0
    bne @L_80095C74
    li r0, 0x0
    b @L_80095D18
@L_80095C74:
    mr r3, r24
    bl fn_801FCEAC
    mr r25, r3
    b @L_80095CB0
@L_80095C84:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_80095CA8
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    mr r25, r3
    b @L_80095CB0
@L_80095CA8:
    bl fn_8006AEEC
    mr r25, r3
@L_80095CB0:
    mr r3, r25
    bl fn_8012AC54
    mr r24, r3
    mr r3, r25
    bl fn_8012AC3C
    mr r25, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x75
    li r6, 0x0
    bl fn_8012640C
    cmplw r25, r3
    bne @L_80095D14
    mr r3, r31
    li r4, 0x0
    li r5, 0x76
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    mr r3, r24
    bl fn_800F9EE4
    cmpwi r3, 0x0
    bne @L_80095D14
    li r0, 0x1
    b @L_80095D18
@L_80095D14:
    li r0, 0x0
@L_80095D18:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80095D7C
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0xc6
    bge @L_80095D58
    cmpwi r0, 0xc4
    bge @L_80095D50
    b @L_80095D58
@L_80095D50:
    li r26, 0x2be4
    b @L_80095E64
@L_80095D58:
    cmplwi r27, 0x0
    beq @L_80095E64
    mr r3, r27
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    li r26, 0x2bd7
    b @L_80095E64
@L_80095D7C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x70
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x2
    bl fn_80135938
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0xb
    beq @L_80095DB4
    bge @L_80095E64
    cmpwi r0, 0x8
    bge @L_80095E58
    b @L_80095E64
@L_80095DB4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x75
    li r6, 0x0
    bl fn_8012640C
    addis r0, r3, 0x0
    cmplwi r0, 0x911d
    bne @L_80095E0C
    li r3, 0x12ac
    bl fn_800FA280
    mr r25, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x76
    li r6, 0x0
    bl fn_8012640C
    mr r4, r25
    bl fn_800F9EE4
    cmpwi r3, 0x0
    bne @L_80095E0C
    li r0, 0x1
    b @L_80095E10
@L_80095E0C:
    li r0, 0x0
@L_80095E10:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80095E24
    li r26, 0x2be7
    b @L_80095E64
@L_80095E24:
    cmplwi r28, 0xff
    bne @L_80095E34
    li r26, 0x2be5
    b @L_80095E64
@L_80095E34:
    cmplwi r27, 0x0
    beq @L_80095E64
    mr r3, r27
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    li r26, 0x2bd7
    b @L_80095E64
@L_80095E58:
    cmplwi r28, 0xff
    bne @L_80095E64
    li r26, 0x2be5
@L_80095E64:
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    mr r8, r26
    li r3, 0x0
    li r4, 0x0
    bl fn_800FBB34
    b @L_800965B4
@L_80095E84:
    mr r3, r31
    bl fn_8011F77C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x6
    bgt @L_80095EE4
    lis r3, -32721
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -4068
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r27, 0x2bd9
    b @L_80095EE4
    li r27, 0x2bda
    b @L_80095EE4
    li r27, 0x2bdb
    b @L_80095EE4
    li r27, 0x2bdc
    b @L_80095EE4
    li r27, 0x2bdd
    b @L_80095EE4
    li r27, 0x2bde
    b @L_80095EE4
    li r27, 0x2bdf
@L_80095EE4:
    mr r5, r29
    mr r6, r27
    li r3, 0x0
    li r4, 0x0
    bl fn_800FB680
    b @L_800965B4
@L_80095EFC:
    mr r3, r31
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    mr r3, r31
    addi r0, r4, 0x1
    rlwinm r4, r0, 0, 24, 31
    bl fn_801229F4
    mr r26, r3
    cmplwi r26, 0x0
    bne @L_80095F38
    li r4, 0x0
    b @L_80095F50
@L_80095F38:
    mr r3, r31
    li r4, 0x0
    li r5, 0x79
    li r6, 0x0
    bl fn_8012640C
    subf r4, r3, r26
@L_80095F50:
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_800965B4
@L_80095F78:
    mr r3, r31
    li r4, 0x0
    li r5, 0x79
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_800965B4
@L_80095FB8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x8c
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_800965B4
@L_80095FF8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x8b
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_800965B4
@L_80096038:
    mr r3, r31
    li r4, 0x0
    li r5, 0x8a
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_800965B4
@L_80096078:
    mr r3, r31
    li r4, 0x0
    li r5, 0x89
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_800965B4
@L_800960B8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x88
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_800965B4
@L_800960F8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r6, 0x56(r30)
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x37
    li r8, 0xde
    bl fn_800FBB34
    mr r5, r29
    li r3, 0x37
    li r4, 0x0
    li r6, 0x2bd4
    bl fn_800FB680
    mr r3, r31
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_800965B4
@L_80096188:
    mr r3, r31
    bl fn_801248C4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8011CB6C
    bl fn_8011CB3C
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xcf
    bl fn_800FBB34
    b @L_800965B4
@L_800961CC:
    mr r3, r31
    bl fn_801248C4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xcf
    bl fn_800FBB34
    b @L_800965B4
@L_80096210:
    lwz r3, 0x4c(r30)
    bl fn_800FA444
    rlwinm r0, r3, 16, 16, 31
    mr r3, r31
    extsh r0, r0
    mr r24, r0
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80096250
    mr r3, r24
    mr r5, r29
    li r4, 0x0
    li r6, 0x2b70
    bl fn_800FB680
    b @L_800965B4
@L_80096250:
    mr r3, r31
    li r4, 0x0
    li r5, 0x75
    li r6, 0x0
    bl fn_8012640C
    mr r27, r24
    rlwinm r24, r3, 0, 16, 31
    li r28, 0x2710
    li r25, 0x0
    lis r3, 0xcccd
    subi r26, r3, 0x3333
@L_8009627C:
    divwu r4, r24, r28
    li r3, 0x34
    mullw r5, r4, r28
    mulhwu r0, r26, r28
    subf r24, r5, r24
    rlwinm r28, r0, 29, 3, 31
    bl fn_80132A38
    mr r3, r27
    mr r5, r29
    li r4, 0x0
    li r6, 0xca
    bl fn_800FB680
    addi r27, r27, 0xd
    addi r25, r25, 0x1
    cmpwi r25, 0x5
    blt @L_8009627C
    b @L_800965B4
@L_800962C0:
    mr r3, r31
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_800962EC
    li r3, 0x2b70
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    b @L_8009630C
@L_800962EC:
    mr r3, r31
    li r4, 0x0
    li r5, 0x76
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
@L_8009630C:
    lwz r3, 0x4c(r30)
    bl fn_800FA444
    rlwinm r0, r3, 16, 16, 31
    mr r5, r29
    extsh r3, r0
    li r4, 0x0
    li r6, 0xcf
    bl fn_800FB680
    b @L_800965B4
@L_80096330:
    mr r3, r31
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    rlwinm r24, r3, 0, 24, 31
    mr r3, r31
    addi r0, r24, 0x1
    rlwinm r4, r0, 0, 24, 31
    bl fn_801229F4
    mr r27, r3
    cmplwi r27, 0x0
    beq @L_800965B4
    mr r3, r31
    mr r4, r24
    bl fn_801229F4
    mr r26, r3
    mr r3, r31
    subf r24, r26, r27
    li r4, 0x0
    li r5, 0x79
    li r6, 0x0
    bl fn_8012640C
    lha r0, 0x54(r30)
    subf r3, r26, r3
    lha r6, 0x56(r30)
    mr r7, r29
    mullw r0, r3, r0
    mr r8, r28
    li r3, 0x0
    li r4, 0x0
    li r9, 0x117
    li r10, 0x0
    add r5, r24, r0
    subi r0, r5, 0x1
    divwu r0, r0, r24
    extsh r5, r0
    bl fn_80104160
    b @L_800965B4
@L_800963CC:
    cmpwi r4, 0x12b9
    beq @L_80096410
    bge @L_800963F0
    cmpwi r4, 0x595
    beq @L_80096400
    blt @L_80096424
    cmpwi r4, 0x12b8
    bge @L_80096408
    b @L_80096424
@L_800963F0:
    cmpwi r4, 0x12bb
    beq @L_80096420
    bge @L_80096424
    b @L_80096418
@L_80096400:
    li r27, 0x0
    b @L_80096424
@L_80096408:
    li r27, 0x1
    b @L_80096424
@L_80096410:
    li r27, 0x2
    b @L_80096424
@L_80096418:
    li r27, 0x3
    b @L_80096424
@L_80096420:
    li r27, 0x4
@L_80096424:
    mr r3, r31
    li r4, 0x0
    li r5, 0xc4
    li r6, 0x0
    bl fn_8012640C
    rlwinm r24, r3, 0, 16, 31
    cmplwi r24, 0x0
    bne @L_8009644C
    lfs f4, -29848(r2)
    b @L_800964A0
@L_8009644C:
    mr r3, r31
    bl fn_8011FC14
    lis r0, 0x4330
    stw r24, 0xc(r1)
    lfd f2, -29824(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f2
    fcmpo cr0, f1, f0
    ble @L_80096484
    stw r24, 0x14(r1)
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f1, f0, f2
@L_80096484:
    lis r0, 0x4330
    stw r24, 0x1c(r1)
    lfd f2, -29824(r2)
    stw r0, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f2
    fdivs f4, f1, f0
@L_800964A0:
    lfs f0, -29844(r2)
    fcmpo cr0, f4, f0
    cror eq, gt, eq
    bne @L_800964B8
    li r0, 0x4
    b @L_80096504
@L_800964B8:
    lfs f0, -29840(r2)
    fcmpo cr0, f4, f0
    cror eq, gt, eq
    bne @L_800964D0
    li r0, 0x3
    b @L_80096504
@L_800964D0:
    lfs f0, -29836(r2)
    fcmpo cr0, f4, f0
    cror eq, gt, eq
    bne @L_800964E8
    li r0, 0x2
    b @L_80096504
@L_800964E8:
    lfs f0, -29832(r2)
    fcmpo cr0, f4, f0
    cror eq, gt, eq
    bne @L_80096500
    li r0, 0x1
    b @L_80096504
@L_80096500:
    li r0, 0x0
@L_80096504:
    rlwinm r4, r0, 0, 16, 31
    rlwinm r0, r27, 0, 16, 31
    cmplw r4, r0
    ble @L_8009653C
    lha r5, 0x54(r30)
    mr r7, r29
    lha r6, 0x56(r30)
    mr r8, r28
    li r3, 0x0
    li r4, 0x0
    li r9, 0x116
    li r10, 0x0
    bl fn_80104160
    b @L_800965B4
@L_8009653C:
    bne @L_800965B4
    lis r3, 0x4330
    lha r0, 0x54(r30)
    stw r4, 0x1c(r1)
    xoris r0, r0, 0x8000
    lfd f1, -29824(r2)
    stw r3, 0x18(r1)
    lfs f2, -29832(r2)
    lfd f0, 0x18(r1)
    stw r0, 0x14(r1)
    fsubs f0, f0, f1
    lfs f3, -29828(r2)
    stw r3, 0x10(r1)
    lfd f1, -29816(r2)
    fmuls f2, f2, f0
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fsubs f1, f4, f2
    fmuls f1, f3, f1
    fmuls f1, f0, f1
    bl fn_800C46B0
    lha r6, 0x56(r30)
    extsh r5, r3
    mr r7, r29
    mr r8, r28
    li r3, 0x0
    li r4, 0x0
    li r9, 0x116
    li r10, 0x0
    bl fn_80104160
@L_800965B4:
    lmw r24, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
