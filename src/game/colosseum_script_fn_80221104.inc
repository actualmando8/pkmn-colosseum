    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r30, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    rlwinm r3, r25, 0, 24, 31
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x11
    mr r31, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x12
    mr r28, r0
    li r4, 0x0
    bl fn_801F025C
    lis r4, lbl_80379F58@ha
    mr r27, r3
    addi r4, r4, lbl_80379F58@l
    cmplwi r30, 0x11
    addis r4, r4, 0x1
    lbz r3, 0x601e(r4)
    lbz r0, 0x60a4(r4)
    rlwinm r4, r3, 0, 28, 31
    subi r3, r4, 0x1
    subf r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    beq @L_802211AC
    subi r0, r30, 0x1
    cmplwi r0, 0x1
    ble @L_802211AC
    cmplwi r30, 0x1a
    bne @L_802213CC
@L_802211AC:
    cmplwi r30, 0x1
    bne @L_802212BC
    la r5, lbl_80478D78(r13)
    cmplwi r5, 0x0
    beq @L_80221248
    lbz r5, 0x3(r5)
    cmplwi r5, 0x0
    beq @L_8022123C
    subi r0, r5, 0xf
    cmplwi r0, 0xd
    bgt @L_80221230
    lis r5, jumptable_8039A1C8@ha
    rlwinm r0, r0, 2, 0, 29
    addi r5, r5, jumptable_8039A1C8@l
    lwzx r0, r5, r0
    mtctr r0
    bctr
    li r26, 0x26
    b @L_80221250
    li r26, 0x28
    b @L_80221250
    li r26, 0x2a
    b @L_80221250
    li r26, 0x2c
    b @L_80221250
    li r26, 0x27
    b @L_80221250
    li r26, 0x29
    b @L_80221250
    li r26, 0x2b
    b @L_80221250
    li r26, 0x3c
    b @L_80221250
@L_80221230:
    bl fn_80222110
    mr r26, r3
    b @L_80221250
@L_8022123C:
    bl fn_80222110
    mr r26, r3
    b @L_80221250
@L_80221248:
    bl fn_80222110
    mr r26, r3
@L_80221250:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r29, r3
    beq @L_802212BC
    mr r4, r26
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802212BC
    mr r3, r29
    mr r4, r26
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r27
    li r5, 0x1
    bl fn_80265598
@L_802212BC:
    cmplwi r30, 0x1a
    bne @L_80221330
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r29, r3
    beq @L_80221330
    li r4, 0x2d
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221330
    mr r3, r29
    li r4, 0x2d
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r27
    li r5, 0x1
    bl fn_80265598
@L_80221330:
    cmplwi r30, 0x11
    bne @L_802213A8
    mr r3, r31
    li r4, 0x121
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    mr r3, r28
    li r4, 0x121
    li r5, 0x2
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0x121
    li r5, 0x1
    li r6, 0x1
    bl fn_802085C4
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r28
    li r4, 0x121
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
@L_802213A8:
    cmplwi r30, 0x2
    bne @L_802220FC
    mr r4, r31
    mr r5, r31
    li r3, 0xa4
    li r6, 0x0
    li r7, 0x0
    bl fn_8020F108
    b @L_802220FC
@L_802213CC:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_802213F0
    lis r4, lbl_80378964@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378964@l
    li r5, 0x0
    bl fn_80211B94
    b @L_802220FC
@L_802213F0:
    subi r0, r30, 0xa
    cmplwi r0, 0x3
    ble @L_80221410
    subi r0, r30, 0x20
    cmplwi r0, 0x2
    ble @L_80221410
    cmplwi r30, 0x1e
    bne @L_80221804
@L_80221410:
    cmplwi r30, 0xa
    bne @L_80221450
    mr r3, r31
    li r4, 0xf0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0xf0
    li r5, 0x1
    li r6, 0x0
    bl fn_802085C4
@L_80221450:
    cmplwi r30, 0xb
    bne @L_80221490
    mr r3, r31
    li r4, 0xf1
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0xf1
    li r5, 0x1
    li r6, 0x0
    bl fn_802085C4
@L_80221490:
    cmplwi r30, 0xc
    bne @L_802214D0
    mr r3, r31
    li r4, 0xc9
    li r5, 0x2
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0xc9
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
@L_802214D0:
    cmplwi r30, 0xd
    bne @L_80221510
    mr r3, r31
    li r4, 0x102
    li r5, 0x2
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0x102
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
@L_80221510:
    cmplwi r30, 0x20
    bne @L_80221550
    mr r3, r31
    li r4, 0xf0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0xf0
    li r5, 0x1
    li r6, 0x0
    bl fn_802085C4
@L_80221550:
    cmplwi r30, 0x22
    bne @L_80221590
    mr r3, r31
    li r4, 0xc9
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0xc9
    li r5, 0x1
    li r6, 0x0
    bl fn_802085C4
@L_80221590:
    cmplwi r30, 0x21
    bne @L_802215D0
    mr r3, r31
    li r4, 0xf1
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0xf1
    li r5, 0x1
    li r6, 0x0
    bl fn_802085C4
@L_802215D0:
    cmplwi r30, 0x1e
    bne @L_802220FC
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022164C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_802220FC
    li r4, 0x80
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802220FC
    mr r3, r27
    li r4, 0x80
    li r5, 0x4
    bl fn_801DA9E8
    b @L_802220FC
@L_8022164C:
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802216C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_802220FC
    li r4, 0x7f
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802220FC
    mr r3, r27
    li r4, 0x7f
    li r5, 0x4
    bl fn_801DA9E8
    b @L_802220FC
@L_802216C0:
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x3
    bne @L_80221734
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_802220FC
    li r4, 0x7e
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802220FC
    mr r3, r27
    li r4, 0x7e
    li r5, 0x4
    bl fn_801DA9E8
    b @L_802220FC
@L_80221734:
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x4
    bne @L_802217A8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_802220FC
    li r4, 0x7d
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802220FC
    mr r3, r27
    li r4, 0x7d
    li r5, 0x4
    bl fn_801DA9E8
    b @L_802220FC
@L_802217A8:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_802220FC
    li r4, 0x9d
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802220FC
    mr r3, r27
    li r4, 0x9d
    li r5, 0x4
    bl fn_801DA9E8
    b @L_802220FC
@L_80221804:
    cmplwi r30, 0x17
    bne @L_80221878
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r28, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r26, r3
    beq @L_80221878
    li r4, 0x57
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221878
    mr r3, r26
    li r4, 0x57
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r28
    li r5, 0x1
    bl fn_80265598
@L_80221878:
    cmplwi r30, 0x1f
    bne @L_802218D8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r26, r3
    beq @L_802218D8
    li r4, 0x9d
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802218D8
    mr r3, r26
    li r4, 0x9d
    li r5, 0x4
    bl fn_801DA9E8
@L_802218D8:
    cmplwi r30, 0x18
    bne @L_8022194C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    beq @L_8022194C
    li r4, 0x58
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022194C
    mr r3, r28
    li r4, 0x58
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
@L_8022194C:
    cmplwi r30, 0x23
    bne @L_802219C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    beq @L_802219C0
    li r4, 0xa3
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802219C0
    mr r3, r28
    li r4, 0xa3
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
@L_802219C0:
    cmplwi r30, 0x24
    bne @L_80221A34
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    beq @L_80221A34
    li r4, 0xd8
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221A34
    mr r3, r28
    li r4, 0xd8
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
@L_80221A34:
    cmplwi r30, 0x19
    bne @L_80221A74
    mr r3, r31
    li r4, 0x1
    li r5, 0x2
    li r6, 0x0
    bl fn_80208750
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0x1
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
@L_80221A74:
    cmplwi r30, 0xe
    bne @L_80221AE8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    beq @L_80221AE8
    li r4, 0x38
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221AE8
    mr r3, r28
    li r4, 0x38
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
@L_80221AE8:
    cmplwi r30, 0x16
    bne @L_80221B28
    mr r3, r31
    li r4, 0x111
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0x111
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
@L_80221B28:
    cmplwi r30, 0x12
    bne @L_80221B68
    mr r3, r31
    li r4, 0xf8
    li r5, 0x2
    li r6, 0x0
    bl fn_80208750
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0xf8
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
@L_80221B68:
    cmplwi r30, 0x13
    bne @L_80221BA8
    mr r3, r27
    li r4, 0x161
    li r5, 0x2
    li r6, 0x0
    bl fn_80208750
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r27
    li r4, 0x161
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
@L_80221BA8:
    cmplwi r30, 0x15
    bne @L_80221C1C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_80221C1C
    li r4, 0x3b
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221C1C
    mr r3, r27
    li r4, 0x3b
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
@L_80221C1C:
    cmplwi r30, 0x7
    bne @L_80221C90
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_80221C90
    li r4, 0x39
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221C90
    mr r3, r27
    li r4, 0x39
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
@L_80221C90:
    cmplwi r30, 0x1b
    bne @L_80221D04
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_80221D04
    li r4, 0x32
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221D04
    mr r3, r27
    li r4, 0x32
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
@L_80221D04:
    cmplwi r30, 0x1c
    bne @L_80221D44
    mr r3, r31
    li r4, 0x36
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0x36
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
@L_80221D44:
    cmplwi r30, 0x1d
    bne @L_80221D84
    mr r3, r31
    li r4, 0x74
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0x74
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
@L_80221D84:
    cmplwi r30, 0x9
    bne @L_80221DF8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_80221DF8
    li r4, 0x86
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221DF8
    mr r3, r27
    li r4, 0x86
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r26
    li r5, 0x1
    bl fn_80265598
@L_80221DF8:
    cmplwi r30, 0x6
    bne @L_80221E4C
    mr r3, r31
    li r4, 0xe
    bl fn_80201C58
    mr r0, r3
    mr r3, r31
    mr r26, r0
    li r5, 0x2
    mr r4, r26
    li r6, 0x0
    bl fn_80208750
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    mr r4, r26
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
@L_80221E4C:
    cmplwi r30, 0x10
    bne @L_80221EEC
    mr r3, r31
    bl fn_80205184
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x157
    bne @L_80221EA0
    mr r3, r31
    li r4, 0x157
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0x157
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
@L_80221EA0:
    mr r3, r31
    bl fn_80205184
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xa8
    bne @L_80221EEC
    mr r3, r31
    li r4, 0xa8
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0xa8
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
@L_80221EEC:
    cmplwi r30, 0x0
    bne @L_802220BC
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm r26, r3, 0, 24, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r28, r0
    mr r4, r26
    bl fn_80207C6C
    cmpwi r26, 0x3
    mr r25, r3
    beq @L_80221F70
    bge @L_80221F4C
    cmpwi r26, 0x1
    beq @L_80221F58
    bge @L_80221F60
    b @L_80221F70
@L_80221F4C:
    cmpwi r26, 0x5
    bge @L_80221F70
    b @L_80221F68
@L_80221F58:
    li r27, 0x84
    b @L_80221F74
@L_80221F60:
    li r27, 0x85
    b @L_80221F74
@L_80221F68:
    li r27, 0x82
    b @L_80221F74
@L_80221F70:
    li r27, 0x83
@L_80221F74:
    mr r3, r28
    li r4, 0x81
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r25
    mr r4, r27
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r26, r3
    beq @L_80222034
    li r4, 0x81
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80222034
    mr r3, r26
    li r4, 0x81
    li r5, 0x4
    bl fn_801DA9E8
@L_80222004:
    mr r3, r26
    li r4, 0x81
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80222024
    bl fn_800F0308
    b @L_80222004
@L_80222024:
    mr r3, r26
    li r4, 0x81
    li r5, 0x4
    bl fn_801DA8C4
@L_80222034:
    mr r3, r28
    mr r4, r25
    bl fn_801C3D64
    bl fn_801C3430
    mr r3, r31
    mr r7, r25
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_801254B4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r26, r3
    beq @L_802220B4
    mr r4, r27
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802220B4
    mr r3, r26
    mr r4, r27
    li r5, 0x4
    bl fn_801DA9E8
@L_802220B4:
    mr r3, r28
    bl fn_801DB100
@L_802220BC:
    cmplwi r30, 0x14
    bne @L_802220FC
    mr r3, r31
    li r4, 0x75
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    li r4, 0x75
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
@L_802220FC:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
