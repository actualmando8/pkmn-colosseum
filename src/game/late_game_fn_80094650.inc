    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr r27, r3
    mr r31, r4
    lis r3, -32704
    addi r3, r3, -19584
    lwz r29, 0xc(r3)
    cmplwi r29, 0x0
    beq @L_80095668
    lha r0, 0x6(r31)
    li r30, 0x1
    cmpwi r0, 0x1b8
    bge @L_800946B4
    cmpwi r0, 0x18b
    bge @L_800946A8
    cmpwi r0, 0x182
    bge @L_8009473C
    cmpwi r0, 0x170
    bge @L_800946D0
    b @L_8009473C
@L_800946A8:
    cmpwi r0, 0x191
    bge @L_8009473C
    b @L_800946D0
@L_800946B4:
    cmpwi r0, 0x1d3
    bge @L_800946C8
    cmpwi r0, 0x1ca
    bge @L_8009473C
    b @L_800946D0
@L_800946C8:
    cmpwi r0, 0x1d9
    bge @L_8009473C
@L_800946D0:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x1(r3)
    cmpwi r0, 0x7
    beq @L_800946FC
    bge @L_8009472C
    cmpwi r0, 0x5
    bge @L_8009472C
    cmpwi r0, 0x3
    bge @L_800946FC
    b @L_8009472C
@L_800946FC:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x2(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80094724
    cmpwi r0, 0x4
    bgt @L_80094724
    li r30, 0x1
    b @L_80094730
@L_80094724:
    li r30, 0x0
    b @L_80094730
@L_8009472C:
    li r30, 0x0
@L_80094730:
    mr r3, r31
    mr r4, r30
    bl fn_80109220
@L_8009473C:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80095668
    lha r4, 0x6(r31)
    li r0, -0x100
    lbz r3, 0x8b(r27)
    cmpwi r4, 0x1c1
    or r30, r3, r0
    bge @L_800947E0
    cmpwi r4, 0x18d
    beq @L_80095134
    bge @L_800947A8
    cmpwi r4, 0x181
    beq @L_80095668
    bge @L_80094790
    cmpwi r4, 0x170
    beq @L_80094F3C
    blt @L_80095668
    cmpwi r4, 0x179
    bge @L_800952B4
    b @L_80095490
@L_80094790:
    cmpwi r4, 0x18b
    beq @L_80095010
    bge @L_800950A8
    cmpwi r4, 0x187
    bge @L_80095668
    b @L_800949F4
@L_800947A8:
    cmpwi r4, 0x196
    bge @L_800947C8
    cmpwi r4, 0x190
    beq @L_80095668
    bge @L_80094B58
    cmpwi r4, 0x18f
    bge @L_800951F4
    b @L_80095668
@L_800947C8:
    cmpwi r4, 0x1b8
    beq @L_80094F3C
    bge @L_80095490
    cmpwi r4, 0x19b
    bge @L_80095668
    b @L_80094CB8
@L_800947E0:
    cmpwi r4, 0x1d7
    beq @L_800951F4
    bge @L_80094820
    cmpwi r4, 0x1d3
    beq @L_80095010
    bge @L_80094810
    cmpwi r4, 0x1c9
    beq @L_80095668
    blt @L_800952B4
    cmpwi r4, 0x1ce
    bge @L_80095668
    b @L_800949F4
@L_80094810:
    cmpwi r4, 0x1d5
    beq @L_80095134
    bge @L_80095668
    b @L_800950A8
@L_80094820:
    cmpwi r4, 0x59b
    bge @L_80094848
    cmpwi r4, 0x1dd
    bge @L_8009483C
    cmpwi r4, 0x1d9
    bge @L_80094B58
    b @L_80095668
@L_8009483C:
    cmpwi r4, 0x1e1
    bge @L_80095668
    b @L_80094CB8
@L_80094848:
    cmpwi r4, 0x12b3
    bge @L_8009485C
    cmpwi r4, 0x59f
    bge @L_80095668
    b @L_80094864
@L_8009485C:
    cmpwi r4, 0x12b8
    bge @L_80095668
@L_80094864:
    lwz r3, -29856(r2)
    cmpwi r4, 0x12b3
    lwz r0, -29852(r2)
    stw r3, 0x18(r1)
    stw r0, 0x14(r1)
    beq @L_800948F4
    bge @L_800948A8
    cmpwi r4, 0x59d
    beq @L_800948DC
    bge @L_8009489C
    cmpwi r4, 0x59b
    beq @L_800948CC
    bge @L_800948D4
    b @L_80094910
@L_8009489C:
    cmpwi r4, 0x59f
    bge @L_80094910
    b @L_800948E4
@L_800948A8:
    cmpwi r4, 0x12b6
    beq @L_8009490C
    bge @L_800948C0
    cmpwi r4, 0x12b5
    bge @L_80094904
    b @L_800948FC
@L_800948C0:
    cmpwi r4, 0x12b8
    bge @L_80094910
    b @L_800948EC
@L_800948CC:
    li r28, 0x0
    b @L_80094910
@L_800948D4:
    li r28, 0x1
    b @L_80094910
@L_800948DC:
    li r28, 0x2
    b @L_80094910
@L_800948E4:
    li r28, 0x3
    b @L_80094910
@L_800948EC:
    li r28, 0x0
    b @L_80094910
@L_800948F4:
    li r28, 0x1
    b @L_80094910
@L_800948FC:
    li r28, 0x2
    b @L_80094910
@L_80094904:
    li r28, 0x3
    b @L_80094910
@L_8009490C:
    li r28, 0x4
@L_80094910:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r0, 0x1(r3)
    cmpwi r0, 0x4
    beq @L_80094940
    bge @L_80094934
    cmpwi r0, 0x3
    bge @L_800949B4
    b @L_80095668
@L_80094934:
    cmpwi r0, 0x7
    beq @L_800949B4
    b @L_80095668
@L_80094940:
    lbz r3, 0x3(r3)
    extsb r0, r28
    extsb r3, r3
    cmpw r3, r0
    bne @L_80094974
    lwz r0, 0x18(r1)
    addi r7, r1, 0x10
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x10(r1)
    lha r5, 0x54(r31)
    lha r6, 0x56(r31)
    bl fn_8001E58C
@L_80094974:
    lis r3, -32704
    extsb r0, r28
    addi r3, r3, -19584
    lbz r3, 0x2(r3)
    extsb r3, r3
    cmpw r3, r0
    bne @L_80095668
    lwz r0, 0x14(r1)
    addi r7, r1, 0xc
    li r3, 0x0
    li r4, 0x0
    stw r0, 0xc(r1)
    lha r5, 0x54(r31)
    lha r6, 0x56(r31)
    bl fn_8001E58C
    b @L_80095668
@L_800949B4:
    lis r3, -32704
    extsb r0, r28
    addi r3, r3, -19584
    lbz r3, 0x2(r3)
    extsb r3, r3
    cmpw r3, r0
    bne @L_80095668
    lwz r0, 0x14(r1)
    addi r7, r1, 0x8
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x8(r1)
    lha r5, 0x54(r31)
    lha r6, 0x56(r31)
    bl fn_8001E58C
    b @L_80095668
@L_800949F4:
    cmpwi r4, 0x186
    beq @L_80094A68
    bge @L_80094A24
    cmpwi r4, 0x183
    beq @L_80094A80
    bge @L_80094A18
    cmpwi r4, 0x182
    bge @L_80094A88
    b @L_80094A8C
@L_80094A18:
    cmpwi r4, 0x185
    bge @L_80094A70
    b @L_80094A78
@L_80094A24:
    cmpwi r4, 0x1cc
    beq @L_80094A50
    bge @L_80094A40
    cmpwi r4, 0x1ca
    beq @L_80094A60
    bge @L_80094A58
    b @L_80094A8C
@L_80094A40:
    cmpwi r4, 0x1ce
    bge @L_80094A8C
    li r28, 0x0
    b @L_80094A8C
@L_80094A50:
    li r28, 0x1
    b @L_80094A8C
@L_80094A58:
    li r28, 0x2
    b @L_80094A8C
@L_80094A60:
    li r28, 0x3
    b @L_80094A8C
@L_80094A68:
    li r28, 0x0
    b @L_80094A8C
@L_80094A70:
    li r28, 0x1
    b @L_80094A8C
@L_80094A78:
    li r28, 0x2
    b @L_80094A8C
@L_80094A80:
    li r28, 0x3
    b @L_80094A8C
@L_80094A88:
    li r28, 0x4
@L_80094A8C:
    rlwinm r30, r28, 0, 16, 31
    cmplwi r30, 0x4
    bne @L_80094AA8
    lis r3, -32704
    addi r3, r3, -19584
    lhz r28, 0x18(r3)
    b @L_80094ADC
@L_80094AA8:
    mr r3, r29
    mr r6, r30
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    mr r3, r29
    mr r4, r30
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80094ADC
    li r28, 0x0
@L_80094ADC:
    rlwinm r0, r28, 0, 16, 31
    cmpwi r0, 0x164
    beq @L_80094B04
    bge @L_80094AF8
    cmpwi r0, 0x0
    beq @L_80094B04
    b @L_80094B14
@L_80094AF8:
    cmpwi r0, 0x166
    bge @L_80094B14
    b @L_80094B0C
@L_80094B04:
    li r0, 0x0
    b @L_80094B34
@L_80094B0C:
    li r0, 0x5d
    b @L_80094B34
@L_80094B14:
    mr r4, r28
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8010C46C
    rlwinm r0, r3, 0, 16, 31
@L_80094B34:
    cmplwi r0, 0x0
    beq @L_80095668
    mr r5, r27
    rlwinm r6, r0, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80095668
@L_80094B58:
    cmpwi r4, 0x195
    beq @L_80094BCC
    bge @L_80094B88
    cmpwi r4, 0x192
    beq @L_80094BE4
    bge @L_80094B7C
    cmpwi r4, 0x191
    bge @L_80094BEC
    b @L_80094BF0
@L_80094B7C:
    cmpwi r4, 0x194
    bge @L_80094BD4
    b @L_80094BDC
@L_80094B88:
    cmpwi r4, 0x1db
    beq @L_80094BB4
    bge @L_80094BA4
    cmpwi r4, 0x1d9
    beq @L_80094BC4
    bge @L_80094BBC
    b @L_80094BF0
@L_80094BA4:
    cmpwi r4, 0x1dd
    bge @L_80094BF0
    li r28, 0x0
    b @L_80094BF0
@L_80094BB4:
    li r28, 0x1
    b @L_80094BF0
@L_80094BBC:
    li r28, 0x2
    b @L_80094BF0
@L_80094BC4:
    li r28, 0x3
    b @L_80094BF0
@L_80094BCC:
    li r28, 0x0
    b @L_80094BF0
@L_80094BD4:
    li r28, 0x1
    b @L_80094BF0
@L_80094BDC:
    li r28, 0x2
    b @L_80094BF0
@L_80094BE4:
    li r28, 0x3
    b @L_80094BF0
@L_80094BEC:
    li r28, 0x4
@L_80094BF0:
    rlwinm r28, r28, 0, 16, 31
    cmplwi r28, 0x4
    bne @L_80094C0C
    lis r3, -32704
    addi r3, r3, -19584
    lhz r27, 0x18(r3)
    b @L_80094C40
@L_80094C0C:
    mr r3, r29
    mr r6, r28
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r27, r3, 0, 16, 31
    mr r3, r29
    mr r4, r28
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80094C40
    li r27, 0x0
@L_80094C40:
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80094C6C
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be0
    bl fn_800FBB34
    b @L_80095668
@L_80094C6C:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x0
    beq @L_80095668
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xe7
    bl fn_800FBB34
    b @L_80095668
@L_80094CB8:
    cmpwi r4, 0x19a
    beq @L_80094D2C
    bge @L_80094CE8
    cmpwi r4, 0x197
    beq @L_80094D44
    bge @L_80094CDC
    cmpwi r4, 0x196
    bge @L_80094D4C
    b @L_80094D50
@L_80094CDC:
    cmpwi r4, 0x199
    bge @L_80094D34
    b @L_80094D3C
@L_80094CE8:
    cmpwi r4, 0x1df
    beq @L_80094D14
    bge @L_80094D04
    cmpwi r4, 0x1dd
    beq @L_80094D24
    bge @L_80094D1C
    b @L_80094D50
@L_80094D04:
    cmpwi r4, 0x1e1
    bge @L_80094D50
    li r28, 0x0
    b @L_80094D50
@L_80094D14:
    li r28, 0x1
    b @L_80094D50
@L_80094D1C:
    li r28, 0x2
    b @L_80094D50
@L_80094D24:
    li r28, 0x3
    b @L_80094D50
@L_80094D2C:
    li r28, 0x0
    b @L_80094D50
@L_80094D34:
    li r28, 0x1
    b @L_80094D50
@L_80094D3C:
    li r28, 0x2
    b @L_80094D50
@L_80094D44:
    li r28, 0x3
    b @L_80094D50
@L_80094D4C:
    li r28, 0x4
@L_80094D50:
    rlwinm r26, r28, 0, 16, 31
    cmplwi r26, 0x4
    bne @L_80094D6C
    lis r3, -32704
    addi r3, r3, -19584
    lhz r27, 0x18(r3)
    b @L_80094DA0
@L_80094D6C:
    mr r3, r29
    mr r6, r26
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r27, r3, 0, 16, 31
    mr r3, r29
    mr r4, r26
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80094DA0
    li r27, 0x0
@L_80094DA0:
    li r3, 0x2bd4
    bl fn_800FA444
    rlwinm r3, r3, 16, 16, 31
    lha r0, 0x54(r31)
    extsh r3, r3
    mr r5, r30
    subf r3, r3, r0
    li r4, 0x0
    rlwinm r0, r3, 1, 31, 31
    li r6, 0x2bd4
    add r0, r0, r3
    srawi r0, r0, 1
    extsh r25, r0
    mr r3, r25
    bl fn_800FB680
    rlwinm r0, r27, 0, 16, 31
    cmpwi r0, 0x164
    beq @L_80094E04
    bge @L_80094DF8
    cmpwi r0, 0x0
    beq @L_80094E04
    b @L_80094E7C
@L_80094DF8:
    cmpwi r0, 0x166
    bge @L_80094E7C
    b @L_80094E40
@L_80094E04:
    lha r6, 0x56(r31)
    mr r5, r25
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be1
    bl fn_800FB8C8
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be1
    bl fn_800FB8C8
    b @L_80095668
@L_80094E40:
    lha r6, 0x56(r31)
    mr r5, r25
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2b6d
    bl fn_800FB8C8
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2b6d
    bl fn_800FB8C8
    b @L_80095668
@L_80094E7C:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_80094EA0
    mr r4, r27
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_8011BEB4
    b @L_80094EB4
@L_80094EA0:
    mr r3, r29
    mr r6, r26
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
@L_80094EB4:
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r6, 0x56(r31)
    mr r5, r25
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FB8C8
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_80094F00
    mr r4, r27
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_8011BEB4
    b @L_80094F10
@L_80094F00:
    mr r3, r29
    mr r4, r28
    bl fn_80123E70
    rlwinm r3, r3, 0, 24, 31
@L_80094F10:
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FB8C8
    b @L_80095668
@L_80094F3C:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r30, 0x2(r3)
    extsb r30, r30
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_80094F60
    lhz r28, 0x18(r3)
    b @L_80094F94
@L_80094F60:
    mr r3, r29
    mr r6, r30
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    mr r3, r29
    mr r4, r30
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80094F94
    li r28, 0x0
@L_80094F94:
    rlwinm r0, r28, 0, 16, 31
    cmpwi r0, 0x164
    beq @L_80094FBC
    bge @L_80094FB0
    cmpwi r0, 0x0
    beq @L_80094FBC
    b @L_80094FCC
@L_80094FB0:
    cmpwi r0, 0x166
    bge @L_80094FCC
    b @L_80094FC4
@L_80094FBC:
    li r0, 0x0
    b @L_80094FEC
@L_80094FC4:
    li r0, 0x5d
    b @L_80094FEC
@L_80094FCC:
    mr r4, r28
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 24, 31
    bl fn_801EE0A8
    rlwinm r0, r3, 0, 16, 31
@L_80094FEC:
    cmplwi r0, 0x0
    beq @L_80095668
    mr r5, r27
    rlwinm r6, r0, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80095668
@L_80095010:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r28, 0x2(r3)
    extsb r28, r28
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_80095034
    lhz r27, 0x18(r3)
    b @L_80095068
@L_80095034:
    mr r3, r29
    mr r6, r28
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r27, r3, 0, 16, 31
    mr r3, r29
    mr r4, r28
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80095068
    li r27, 0x0
@L_80095068:
    mr r4, r27
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
    bl fn_801EE07C
    bl fn_801EE034
    lha r5, 0x54(r31)
    mr r8, r3
    lha r6, 0x56(r31)
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    bl fn_800FBB34
    b @L_80095668
@L_800950A8:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r28, 0x2(r3)
    extsb r28, r28
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_800950CC
    lhz r27, 0x18(r3)
    b @L_80095100
@L_800950CC:
    mr r3, r29
    mr r6, r28
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r27, r3, 0, 16, 31
    mr r3, r29
    mr r4, r28
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80095100
    li r27, 0x0
@L_80095100:
    mr r4, r27
    li r3, 0x0
    li r5, 0x22
    li r6, 0x0
    bl fn_8011BEB4
    lha r5, 0x54(r31)
    mr r8, r3
    lha r6, 0x56(r31)
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    bl fn_800FBB34
    b @L_80095668
@L_80095134:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r28, 0x2(r3)
    extsb r28, r28
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_80095158
    lhz r27, 0x18(r3)
    b @L_8009518C
@L_80095158:
    mr r3, r29
    mr r6, r28
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r27, r3, 0, 16, 31
    mr r3, r29
    mr r4, r28
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8009518C
    li r27, 0x0
@L_8009518C:
    mr r4, r27
    li r3, 0x0
    li r5, 0x6
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x1
    bgt @L_800951C8
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be2
    bl fn_800FB8C8
    b @L_80095668
@L_800951C8:
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FB8C8
    b @L_80095668
@L_800951F4:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r28, 0x2(r3)
    extsb r28, r28
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_80095218
    lhz r27, 0x18(r3)
    b @L_8009524C
@L_80095218:
    mr r3, r29
    mr r6, r28
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r27, r3, 0, 16, 31
    mr r3, r29
    mr r4, r28
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8009524C
    li r27, 0x0
@L_8009524C:
    mr r4, r27
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x1
    bgt @L_80095288
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be2
    bl fn_800FB8C8
    b @L_80095668
@L_80095288:
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FB8C8
    b @L_80095668
@L_800952B4:
    cmpwi r4, 0x1c1
    beq @L_80095374
    bge @L_80095308
    cmpwi r4, 0x17d
    beq @L_80095394
    bge @L_800952F0
    cmpwi r4, 0x17a
    beq @L_800953AC
    bge @L_800952E4
    cmpwi r4, 0x179
    bge @L_800953B4
    b @L_800953B8
@L_800952E4:
    cmpwi r4, 0x17c
    bge @L_8009539C
    b @L_800953A4
@L_800952F0:
    cmpwi r4, 0x180
    beq @L_8009537C
    bge @L_800953B8
    cmpwi r4, 0x17f
    bge @L_80095384
    b @L_8009538C
@L_80095308:
    cmpwi r4, 0x1c6
    beq @L_8009534C
    bge @L_8009532C
    cmpwi r4, 0x1c4
    beq @L_8009535C
    bge @L_80095354
    cmpwi r4, 0x1c3
    bge @L_80095364
    b @L_8009536C
@L_8009532C:
    cmpwi r4, 0x1c8
    beq @L_8009533C
    bge @L_800953B8
    b @L_80095344
@L_8009533C:
    li r28, 0x1
    b @L_800953B8
@L_80095344:
    li r28, 0x2
    b @L_800953B8
@L_8009534C:
    li r28, 0x3
    b @L_800953B8
@L_80095354:
    li r28, 0x4
    b @L_800953B8
@L_8009535C:
    li r28, 0x5
    b @L_800953B8
@L_80095364:
    li r28, 0x6
    b @L_800953B8
@L_8009536C:
    li r28, 0x7
    b @L_800953B8
@L_80095374:
    li r28, 0x8
    b @L_800953B8
@L_8009537C:
    li r28, 0x1
    b @L_800953B8
@L_80095384:
    li r28, 0x2
    b @L_800953B8
@L_8009538C:
    li r28, 0x3
    b @L_800953B8
@L_80095394:
    li r28, 0x4
    b @L_800953B8
@L_8009539C:
    li r28, 0x5
    b @L_800953B8
@L_800953A4:
    li r28, 0x6
    b @L_800953B8
@L_800953AC:
    li r28, 0x7
    b @L_800953B8
@L_800953B4:
    li r28, 0x8
@L_800953B8:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r31, 0x2(r3)
    extsb r31, r31
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_800953DC
    lhz r30, 0x18(r3)
    b @L_80095410
@L_800953DC:
    mr r3, r29
    mr r6, r31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
    mr r3, r29
    mr r4, r31
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80095410
    li r30, 0x0
@L_80095410:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_80095444
    mr r4, r30
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
    bl fn_801EE07C
    bl fn_801EE064
    rlwinm r4, r3, 0, 24, 31
    b @L_80095448
@L_80095444:
    li r4, 0x0
@L_80095448:
    lis r3, 0x6666
    rlwinm r0, r28, 0, 16, 31
    addi r3, r3, 0x6667
    mr r5, r27
    mulhw r6, r3, r4
    li r3, 0x0
    li r4, 0x0
    srawi r6, r6, 2
    rlwinm r7, r6, 1, 31, 31
    add r6, r6, r7
    cmpw r6, r0
    blt @L_80095480
    li r6, 0xf6
    b @L_80095484
@L_80095480:
    li r6, 0xf5
@L_80095484:
    li r7, 0x0
    bl fn_801040F0
    b @L_80095668
@L_80095490:
    cmpwi r4, 0x1b9
    beq @L_80095550
    bge @L_800954E4
    cmpwi r4, 0x175
    beq @L_80095570
    bge @L_800954CC
    cmpwi r4, 0x172
    beq @L_80095588
    bge @L_800954C0
    cmpwi r4, 0x171
    bge @L_80095590
    b @L_80095594
@L_800954C0:
    cmpwi r4, 0x174
    bge @L_80095578
    b @L_80095580
@L_800954CC:
    cmpwi r4, 0x178
    beq @L_80095558
    bge @L_80095594
    cmpwi r4, 0x177
    bge @L_80095560
    b @L_80095568
@L_800954E4:
    cmpwi r4, 0x1be
    beq @L_80095528
    bge @L_80095508
    cmpwi r4, 0x1bc
    beq @L_80095538
    bge @L_80095530
    cmpwi r4, 0x1bb
    bge @L_80095540
    b @L_80095548
@L_80095508:
    cmpwi r4, 0x1c0
    beq @L_80095518
    bge @L_80095594
    b @L_80095520
@L_80095518:
    li r28, 0x1
    b @L_80095594
@L_80095520:
    li r28, 0x2
    b @L_80095594
@L_80095528:
    li r28, 0x3
    b @L_80095594
@L_80095530:
    li r28, 0x4
    b @L_80095594
@L_80095538:
    li r28, 0x5
    b @L_80095594
@L_80095540:
    li r28, 0x6
    b @L_80095594
@L_80095548:
    li r28, 0x7
    b @L_80095594
@L_80095550:
    li r28, 0x8
    b @L_80095594
@L_80095558:
    li r28, 0x1
    b @L_80095594
@L_80095560:
    li r28, 0x2
    b @L_80095594
@L_80095568:
    li r28, 0x3
    b @L_80095594
@L_80095570:
    li r28, 0x4
    b @L_80095594
@L_80095578:
    li r28, 0x5
    b @L_80095594
@L_80095580:
    li r28, 0x6
    b @L_80095594
@L_80095588:
    li r28, 0x7
    b @L_80095594
@L_80095590:
    li r28, 0x8
@L_80095594:
    lis r3, -32704
    addi r3, r3, -19584
    lbz r31, 0x2(r3)
    extsb r31, r31
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_800955B8
    lhz r30, 0x18(r3)
    b @L_800955EC
@L_800955B8:
    mr r3, r29
    mr r6, r31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
    mr r3, r29
    mr r4, r31
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800955EC
    li r30, 0x0
@L_800955EC:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_80095620
    mr r4, r30
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
    bl fn_801EE07C
    bl fn_801EE04C
    rlwinm r4, r3, 0, 24, 31
    b @L_80095624
@L_80095620:
    li r4, 0x0
@L_80095624:
    lis r3, 0x6666
    rlwinm r0, r28, 0, 16, 31
    addi r3, r3, 0x6667
    mr r5, r27
    mulhw r6, r3, r4
    li r3, 0x0
    li r4, 0x0
    srawi r6, r6, 2
    rlwinm r7, r6, 1, 31, 31
    add r6, r6, r7
    cmpw r6, r0
    blt @L_8009565C
    li r6, 0xf7
    b @L_80095660
@L_8009565C:
    li r6, 0xf5
@L_80095660:
    li r7, 0x0
    bl fn_801040F0
@L_80095668:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
