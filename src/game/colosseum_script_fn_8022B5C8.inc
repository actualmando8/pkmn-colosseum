    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    li r30, 0x0
    mr r31, r3
    bl fn_80205B8C
    mr r0, r3
    mr r3, r31
    mr r29, r0
    bl fn_802040E8
    mr r26, r3
    mr r3, r31
    bl fn_80203FE4
    mr r27, r3
    mr r3, r31
    bl fn_80203EDC
    mr r3, r31
    li r28, 0x0
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022B628
    li r3, 0x1
    b @L_8022BB70
@L_8022B628:
    rlwinm r7, r26, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x1c
    bgt @L_8022BAE8
    lis r3, jumptable_8039A314@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A314@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r31
    li r26, 0x0
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B694
    mr r3, r31
    li r4, 0x5
    bl fn_80202810
    lis r3, lbl_80379A3C@ha
    addi r26, r3, lbl_80379A3C@l
@L_8022B694:
    cmplwi r26, 0x0
    mr r28, r26
    beq @L_8022BAE8
    li r30, 0x1
    b @L_8022BAE8
    mr r3, r31
    li r26, 0x0
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B6E4
    mr r3, r31
    li r4, 0x3
    bl fn_80202810
    mr r3, r31
    li r4, 0x4
    bl fn_80202810
    lis r3, lbl_80379A5A@ha
    addi r26, r3, lbl_80379A5A@l
@L_8022B6E4:
    cmplwi r26, 0x0
    mr r28, r26
    beq @L_8022B6F8
    li r30, 0x1
    b @L_8022BAE8
@L_8022B6F8:
    mr r3, r31
    li r26, 0x0
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B734
    mr r3, r31
    li r4, 0x4
    bl fn_80202810
    mr r3, r31
    li r4, 0x3
    bl fn_80202810
    lis r3, lbl_80379A5A@ha
    addi r26, r3, lbl_80379A5A@l
@L_8022B734:
    cmplwi r26, 0x0
    mr r28, r26
    beq @L_8022BAE8
    li r30, 0x1
    b @L_8022BAE8
    mr r3, r31
    li r26, 0x0
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B778
    mr r3, r31
    li r4, 0x6
    bl fn_80202810
    lis r3, lbl_80379A78@ha
    addi r26, r3, lbl_80379A78@l
@L_8022B778:
    cmplwi r26, 0x0
    mr r28, r26
    beq @L_8022BAE8
    li r30, 0x1
    b @L_8022BAE8
    mr r3, r31
    li r26, 0x0
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B7BC
    mr r3, r31
    li r4, 0x7
    bl fn_80202810
    lis r3, lbl_80379A96@ha
    addi r26, r3, lbl_80379A96@l
@L_8022B7BC:
    cmplwi r26, 0x0
    mr r28, r26
    beq @L_8022BAE8
    li r30, 0x1
    b @L_8022BAE8
    mr r3, r31
    li r26, 0x0
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B80C
    mr r3, r31
    li r4, 0x8
    bl fn_80202810
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    lis r3, lbl_80379AB4@ha
    addi r26, r3, lbl_80379AB4@l
@L_8022B80C:
    cmplwi r26, 0x0
    mr r28, r26
    beq @L_8022BAE8
    li r30, 0x1
    b @L_8022BAE8
    mr r3, r31
    li r26, 0x0
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B850
    mr r3, r31
    li r4, 0x9
    bl fn_80202810
    lis r3, lbl_80379AD2@ha
    addi r26, r3, lbl_80379AD2@l
@L_8022B850:
    cmplwi r26, 0x0
    mr r28, r26
    beq @L_8022BAE8
    li r30, 0x2
    b @L_8022BAE8
    mr r3, r31
    li r26, 0x0
    li r4, 0xa
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B894
    mr r3, r31
    li r4, 0xa
    bl fn_80202810
    lis r3, lbl_80379AEE@ha
    addi r26, r3, lbl_80379AEE@l
@L_8022B894:
    cmplwi r26, 0x0
    mr r28, r26
    beq @L_8022BAE8
    li r3, 0xa
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    li r30, 0x2
    b @L_8022BAE8
    mr r3, r31
    bl fn_80203CCC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022B8F4
    mr r3, r31
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BAE8
@L_8022B8F4:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    mr r3, r31
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022B930
    mr r3, r31
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B948
@L_8022B930:
    li r3, 0x3
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
@L_8022B948:
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B984
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    li r3, 0x8
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
@L_8022B984:
    mr r3, r31
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B9B4
    li r3, 0x5
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
@L_8022B9B4:
    mr r3, r31
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B9E4
    li r3, 0x6
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
@L_8022B9E4:
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BA14
    li r3, 0x7
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
@L_8022BA14:
    mr r3, r31
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BA44
    li r3, 0x9
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
@L_8022BA44:
    mr r3, r29
    bl fn_801252E0
    mr r3, r31
    li r4, 0x9
    bl fn_80202810
    mr r3, r31
    li r4, 0x0
    bl fn_80202998
    lis r3, lbl_80379AEE@ha
    li r30, 0x1
    addi r0, r3, lbl_80379AEE@l
    mr r28, r0
    b @L_8022BAE8
    lis r3, lbl_80279FD0@ha
    li r26, 0x0
    addi r29, r3, lbl_80279FD0@l
    b @L_8022BAC8
@L_8022BA88:
    clrlslwi r27, r26, 16, 1
    mr r3, r31
    lhzx r5, r29, r27
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x6
    bge @L_8022BAC4
    lhzx r5, r29, r27
    mr r3, r31
    li r4, 0x0
    li r6, 0x0
    li r7, 0x6
    bl fn_801254B4
    li r30, 0x5
@L_8022BAC4:
    addi r26, r26, 0x1
@L_8022BAC8:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x7
    blt @L_8022BA88
    rlwinm. r0, r30, 0, 24, 31
    beq @L_8022BAE8
    lis r3, lbl_80379B0C@ha
    addi r0, r3, lbl_80379B0C@l
    mr r28, r0
@L_8022BAE8:
    rlwinm. r0, r30, 0, 24, 31
    beq @L_8022BB6C
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801F4C14
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BB54
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BB54
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022BB54:
    cmplwi r28, 0x0
    beq @L_8022BB6C
    lwz r3, lbl_8047B62C(r13)
    mr r4, r28
    li r5, 0x0
    bl fn_80211B94
@L_8022BB6C:
    li r3, 0x1
@L_8022BB70:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
