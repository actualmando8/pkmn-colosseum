    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r31, r4
    mr r29, r3
    mr r25, r5
    mr r3, r31
    bl fn_8020E204
    bl fn_8020E1A4
    clrlwi r27, r3, 24
    li r3, 0x0
    bl fn_801F18DC
    clrlwi. r0, r3, 24
    beq @L_8026328C
    mr r4, r29
    mr r5, r31
    li r24, 0x100
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_802631CC
    li r3, 0x0
    b @L_80263228
@L_802631CC:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r26, r3, 16
    mr r3, r29
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_802631FC
    li r3, 0x0
    b @L_80263228
@L_802631FC:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263214
    li r3, 0x0
    b @L_80263228
@L_80263214:
    mr r4, r26
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x2
    bl fn_801F76B8
@L_80263228:
    cmpwi r3, 0xf3
    beq @L_80263260
    bge @L_80263244
    cmpwi r3, 0xf1
    beq @L_80263250
    bge @L_80263258
    b @L_8026326C
@L_80263244:
    cmpwi r3, 0xf5
    bge @L_8026326C
    b @L_80263268
@L_80263250:
    li r24, 0x100
    b @L_8026326C
@L_80263258:
    li r24, 0x101
    b @L_8026326C
@L_80263260:
    li r24, 0x102
    b @L_8026326C
@L_80263268:
    li r24, 0x103
@L_8026326C:
    mr r3, r24
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
@L_8026328C:
    li r30, 0x0
    li r26, 0x0
    stw r30, 0x8(r1)
    b @L_802639AC
@L_8026329C:
    mr r3, r29
    mr r4, r30
    bl fn_801F981C
    mr. r28, r3
    bne @L_802632B8
    mr r26, r30
    b @L_802639A8
@L_802632B8:
    li r4, 0x1
    bl fn_80205C24
    clrlwi. r0, r3, 24
    bne @L_802632D0
    mr r26, r30
    b @L_802639A8
@L_802632D0:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_802632EC
@L_802632E0:
    mr r3, r29
    bl fn_801F9790
    b @L_802639B8
@L_802632EC:
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80263330
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80263330
@L_80263310:
    mr r3, r28
    bl fn_80207760
    mr r3, r29
    mr r4, r28
    mr r5, r31
    bl fn_801F9130
    mr r26, r30
    b @L_802639A8
@L_80263330:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802633C8
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80263364
    li r3, 0x0
    b @L_802633C0
@L_80263364:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r24, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_80263394
    li r3, 0x0
    b @L_802633C0
@L_80263394:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_802633AC
    li r3, 0x0
    b @L_802633C0
@L_802633AC:
    mr r4, r24
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_802633C0:
    li r4, 0x1
    bl fn_80011D9C
@L_802633C8:
    li r0, 0x0
    stw r0, 0x8(r1)
@L_802633D0:
    mr r3, r28
    bl fn_80207760
    mr r4, r28
    addi r5, r1, 0xc
    li r3, 0x0
    bl fn_801F1918
    li r3, 0x0
    bl fn_801F18DC
    stb r3, 0x23(r1)
    addi r3, r1, 0xc
    addi r4, r1, 0x8
    li r5, 0x1
    bl fn_80011A1C
    mr r24, r3
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_802634BC
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802634B0
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_8026344C
    li r3, 0x0
    b @L_802634A8
@L_8026344C:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r24, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_8026347C
    li r3, 0x0
    b @L_802634A8
@L_8026347C:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263494
    li r3, 0x0
    b @L_802634A8
@L_80263494:
    mr r4, r24
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_802634A8:
    li r4, 0x0
    bl fn_80011D9C
@L_802634B0:
    li r3, 0x1
    bl fn_800119A8
    b @L_802632E0
@L_802634BC:
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8026358C
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8026358C
    cmpwi r24, 0x0
    bge @L_8026358C
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80263580
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_8026351C
    li r3, 0x0
    b @L_80263578
@L_8026351C:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r24, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_8026354C
    li r3, 0x0
    b @L_80263578
@L_8026354C:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263564
    li r3, 0x0
    b @L_80263578
@L_80263564:
    mr r4, r24
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_80263578:
    li r4, 0x0
    bl fn_80011D9C
@L_80263580:
    li r3, 0x1
    bl fn_800119A8
    b @L_80263310
@L_8026358C:
    cmpwi r24, 0x0
    blt @L_8026359C
    li r3, 0x1
    bl fn_800119A8
@L_8026359C:
    mr r3, r29
    mr r4, r28
    mr r5, r31
    mr r6, r24
    bl fn_80263BC8
    mr r24, r3
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_80263668
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8026365C
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_802635F8
    li r3, 0x0
    b @L_80263654
@L_802635F8:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r24, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_80263628
    li r3, 0x0
    b @L_80263654
@L_80263628:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263640
    li r3, 0x0
    b @L_80263654
@L_80263640:
    mr r4, r24
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_80263654:
    li r4, 0x0
    bl fn_80011D9C
@L_8026365C:
    li r3, 0x1
    bl fn_800119A8
    b @L_802632E0
@L_80263668:
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80263738
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80263738
    clrlwi. r0, r24, 24
    beq @L_80263738
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8026372C
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_802636C8
    li r3, 0x0
    b @L_80263724
@L_802636C8:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r24, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_802636F8
    li r3, 0x0
    b @L_80263724
@L_802636F8:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263710
    li r3, 0x0
    b @L_80263724
@L_80263710:
    mr r4, r24
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_80263724:
    li r4, 0x0
    bl fn_80011D9C
@L_8026372C:
    li r3, 0x1
    bl fn_800119A8
    b @L_80263310
@L_80263738:
    clrlwi r0, r24, 24
    cmplwi r0, 0x1
    bne @L_80263904
    mr r3, r29
    bl fn_801F7E60
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802633D0
    clrlwi. r3, r30, 16
    beq @L_80263808
    mr r30, r26
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802637FC
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80263798
    li r3, 0x0
    b @L_802637F4
@L_80263798:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r24, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_802637C8
    li r3, 0x0
    b @L_802637F4
@L_802637C8:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_802637E0
    li r3, 0x0
    b @L_802637F4
@L_802637E0:
    mr r4, r24
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_802637F4:
    li r4, 0x0
    bl fn_80011D9C
@L_802637FC:
    li r3, 0x1
    bl fn_800119A8
    b @L_8026329C
@L_80263808:
    clrlwi r0, r25, 24
    cmplwi r0, 0x1
    bne @L_802633D0
    cmplwi r3, 0x0
    bne @L_802633D0
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802638B4
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80263850
    li r3, 0x0
    b @L_802638AC
@L_80263850:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r24, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_80263880
    li r3, 0x0
    b @L_802638AC
@L_80263880:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263898
    li r3, 0x0
    b @L_802638AC
@L_80263898:
    mr r4, r24
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_802638AC:
    li r4, 0x0
    bl fn_80011D9C
@L_802638B4:
    li r3, 0x1
    bl fn_800119A8
    li r24, 0x0
    b @L_802638F0
@L_802638C4:
    mr r3, r29
    mr r4, r24
    bl fn_801F981C
    cmplwi r3, 0x0
    beq @L_802638EC
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
@L_802638EC:
    addi r24, r24, 0x1
@L_802638F0:
    clrlwi r0, r24, 16
    cmplw r0, r27
    blt @L_802638C4
    li r3, 0x0
    b @L_80263BB4
@L_80263904:
    cmplwi r0, 0x2
    beq @L_802633D0
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802639A4
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80263940
    li r3, 0x0
    b @L_8026399C
@L_80263940:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r24, r3, 16
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_80263970
    li r3, 0x0
    b @L_8026399C
@L_80263970:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263988
    li r3, 0x0
    b @L_8026399C
@L_80263988:
    mr r4, r24
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_8026399C:
    li r4, 0x0
    bl fn_80011D9C
@L_802639A4:
    mr r26, r30
@L_802639A8:
    addi r30, r30, 0x1
@L_802639AC:
    clrlwi r0, r30, 16
    cmplw r0, r27
    blt @L_8026329C
@L_802639B8:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi. r0, r3, 24
    beq @L_80263B70
    mr r4, r29
    mr r5, r31
    li r24, 0x100
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_802639EC
    li r3, 0x0
    b @L_80263A48
@L_802639EC:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r25, r3, 16
    mr r3, r29
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_80263A1C
    li r3, 0x0
    b @L_80263A48
@L_80263A1C:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263A34
    li r3, 0x0
    b @L_80263A48
@L_80263A34:
    mr r4, r25
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x2
    bl fn_801F76B8
@L_80263A48:
    cmpwi r3, 0xf3
    beq @L_80263A80
    bge @L_80263A64
    cmpwi r3, 0xf1
    beq @L_80263A70
    bge @L_80263A78
    b @L_80263A8C
@L_80263A64:
    cmpwi r3, 0xf5
    bge @L_80263A8C
    b @L_80263A88
@L_80263A70:
    li r24, 0x100
    b @L_80263A8C
@L_80263A78:
    li r24, 0x101
    b @L_80263A8C
@L_80263A80:
    li r24, 0x102
    b @L_80263A8C
@L_80263A88:
    li r24, 0x103
@L_80263A8C:
    mr r3, r24
    bl fn_80102620
    clrlwi. r0, r3, 24
    beq @L_80263B70
    mr r4, r29
    mr r5, r31
    li r24, 0x100
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80263AC0
    li r3, 0x0
    b @L_80263B1C
@L_80263AC0:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r25, r3, 16
    mr r3, r29
    mr r4, r31
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_80263AF0
    li r3, 0x0
    b @L_80263B1C
@L_80263AF0:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80263B08
    li r3, 0x0
    b @L_80263B1C
@L_80263B08:
    mr r4, r25
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x2
    bl fn_801F76B8
@L_80263B1C:
    cmpwi r3, 0xf3
    beq @L_80263B54
    bge @L_80263B38
    cmpwi r3, 0xf1
    beq @L_80263B44
    bge @L_80263B4C
    b @L_80263B60
@L_80263B38:
    cmpwi r3, 0xf5
    bge @L_80263B60
    b @L_80263B5C
@L_80263B44:
    li r24, 0x100
    b @L_80263B60
@L_80263B4C:
    li r24, 0x101
    b @L_80263B60
@L_80263B54:
    li r24, 0x102
    b @L_80263B60
@L_80263B5C:
    li r24, 0x103
@L_80263B60:
    mr r3, r24
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
@L_80263B70:
    li r24, 0x0
    b @L_80263BA4
@L_80263B78:
    mr r3, r29
    mr r4, r24
    bl fn_801F981C
    cmplwi r3, 0x0
    beq @L_80263BA0
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
@L_80263BA0:
    addi r24, r24, 0x1
@L_80263BA4:
    clrlwi r0, r24, 16
    cmplw r0, r27
    blt @L_80263B78
    li r3, 0x1
@L_80263BB4:
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
