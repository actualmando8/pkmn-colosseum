    nofralloc
    stwu r1, -0x130(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x134(r1)
    stmw r24, 0x110(r1)
    li r25, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r27, r0
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r24, r0
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r24
    mr r29, r0
    bl fn_80205BE8
    li r4, 0x0
    mr r28, r3
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r24, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r26
    bl fn_801440A0
    bl fn_80143DFC
    bl fn_80143A94
    mr. r31, r3
    bne @L_8021D764
    li r31, 0x7
    b @L_8021D89C
@L_8021D764:
    cmplwi r26, 0x13
    bne @L_8021D774
    li r31, 0x1
    b @L_8021D89C
@L_8021D774:
    bl fn_801437E0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021D788
    li r31, 0x2
    b @L_8021D89C
@L_8021D788:
    mr r3, r31
    bl fn_80143940
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021D800
    mr r3, r31
    bl fn_80143918
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021D800
    mr r3, r31
    bl fn_801438F0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021D800
    mr r3, r31
    bl fn_801438C8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021D800
    mr r3, r31
    bl fn_801438A0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021D800
    mr r3, r31
    bl fn_80143878
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D808
@L_8021D800:
    li r31, 0x3
    b @L_8021D89C
@L_8021D808:
    mr r3, r31
    bl fn_80143A44
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D824
    li r31, 0x4
    b @L_8021D89C
@L_8021D824:
    mr r3, r31
    bl fn_80143A28
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021D874
    mr r3, r31
    bl fn_80143A0C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021D874
    mr r3, r31
    bl fn_801439F0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021D874
    mr r3, r31
    bl fn_801439D4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021D874
    mr r3, r31
    bl fn_801439B8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021D87C
@L_8021D874:
    li r31, 0x5
    b @L_8021D89C
@L_8021D87C:
    mr r3, r31
    bl fn_80143990
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D898
    li r31, 0x6
    b @L_8021D89C
@L_8021D898:
    li r31, 0x7
@L_8021D89C:
    mr r3, r30
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr r5, r29
    mr r29, r3
    mr r6, r26
    addi r3, r1, 0x8
    li r4, 0x0
    li r7, 0x0
    bl fn_80144574
    rlwinm r0, r31, 0, 24, 31
    mr r26, r3
    cmplwi r0, 0x1
    beq @L_8021D8E4
    cmplwi r0, 0x2
    bne @L_8021D94C
@L_8021D8E4:
    extsh r3, r26
    li r5, 0x0
    b @L_8021D910
@L_8021D8F0:
    clrlslwi r0, r5, 16, 3
    addi r4, r1, 0x8
    add r4, r4, r0
    lwz r0, 0x0(r4)
    cmpwi r0, 0x15
    bne @L_8021D90C
    lha r25, 0x4(r4)
@L_8021D90C:
    addi r5, r5, 0x1
@L_8021D910:
    rlwinm r0, r5, 0, 16, 31
    cmpw r0, r3
    blt @L_8021D8F0
    mr r3, r28
    mr r7, r24
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_801254B4
    neg r7, r25
    mr r3, r27
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
@L_8021D94C:
    extsh r25, r26
    addi r26, r1, 0x8
    li r27, 0x0
    b @L_8021D994
@L_8021D95C:
    clrlslwi r24, r27, 16, 3
    lwzx r0, r26, r24
    cmpwi r0, 0xf
    bne @L_8021D978
    mr r3, r29
    li r4, 0x2
    bl fn_801DA36C
@L_8021D978:
    lwzx r0, r26, r24
    cmpwi r0, 0x9
    bne @L_8021D990
    mr r3, r29
    li r4, 0x1
    bl fn_801DA36C
@L_8021D990:
    addi r27, r27, 0x1
@L_8021D994:
    rlwinm r0, r27, 0, 16, 31
    cmpw r0, r25
    blt @L_8021D95C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r24, 0x110(r1)
    lwz r0, 0x134(r1)
    mtlr r0
    addi r1, r1, 0x130
    blr
