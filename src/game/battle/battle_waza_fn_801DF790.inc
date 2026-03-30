    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    li r28, 0x1
    li r30, 0x0
    li r29, 0x0
@L_801DF7B0:
    cmplwi r4, 0xf
    bgt @L_801DFC14
    lis r3, -32713
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, 20768
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x3b28
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r4, 0x1
    b @L_801DFC14
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801DF800
    li r4, 0x2
    b @L_801DFC14
@L_801DF800:
    li r4, 0xc
    b @L_801DFC14
    li r3, 0x3b29
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r4, 0x3
    b @L_801DFC14
    li r3, 0x6
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    mr. r31, r3
    blt @L_801DF844
    li r4, 0x4
    b @L_801DFC14
@L_801DF844:
    li r4, 0xc
    b @L_801DFC14
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    mr r27, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DF878
    li r0, 0x0
    b @L_801DF8A8
@L_801DF878:
    mr r3, r27
    bl fn_8011EE40
    rlwinm. r0, r3, 0, 16, 31
    beq @L_801DF8A4
    bl fn_801EEC74
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DF89C
    li r0, 0x1
    b @L_801DF8A8
@L_801DF89C:
    li r0, 0x0
    b @L_801DF8A8
@L_801DF8A4:
    li r0, 0x1
@L_801DF8A8:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_801DF8B8
    li r4, 0x5
    b @L_801DFC14
@L_801DF8B8:
    li r4, 0xd
    b @L_801DFC14
    li r27, 0x0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    mr r25, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DF8F0
    li r27, 0x0
    b @L_801DF918
@L_801DF8F0:
    li r26, 0x0
@L_801DF8F4:
    mr r3, r25
    rlwinm r4, r26, 0, 16, 31
    bl fn_80123CD4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DF90C
    addi r27, r27, 0x1
@L_801DF90C:
    addi r26, r26, 0x1
    cmpwi r26, 0x4
    blt @L_801DF8F4
@L_801DF918:
    cmpwi r27, 0x1
    bne @L_801DF928
    li r4, 0xe
    b @L_801DFC14
@L_801DF928:
    li r4, 0x6
    b @L_801DFC14
    li r3, 0x3b2a
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r4, 0x7
    b @L_801DFC14
    li r25, 0x0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    mr r27, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DF97C
    li r25, 0x0
    b @L_801DF9A4
@L_801DF97C:
    li r26, 0x0
@L_801DF980:
    mr r3, r27
    rlwinm r4, r26, 0, 16, 31
    bl fn_80123CD4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DF998
    addi r25, r25, 0x1
@L_801DF998:
    addi r26, r26, 0x1
    cmpwi r26, 0x4
    blt @L_801DF980
@L_801DF9A4:
    cmpwi r25, 0x1
    bne @L_801DF9B4
    li r4, 0xe
    b @L_801DFC14
@L_801DF9B4:
    rlwinm r3, r31, 0, 24, 31
    bl fn_80097BBC
    mr. r30, r3
    blt @L_801DF9CC
    li r4, 0x8
    b @L_801DFC14
@L_801DF9CC:
    li r4, 0x2
    b @L_801DFC14
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    rlwinm r4, r30, 0, 16, 31
    bl fn_8011F228
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x39
    mr r29, r4
    bl fn_80132A38
    li r3, 0x3b2b
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r4, 0x9
    b @L_801DFC14
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801DFA54
    li r4, 0xa
    b @L_801DFC14
@L_801DFA54:
    li r4, 0x6
    b @L_801DFC14
    li r3, 0x48
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r4, 0xb
    b @L_801DFC14
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    rlwinm r4, r30, 0, 16, 31
    mr r26, r3
    bl fn_80125314
    rlwinm r25, r30, 0, 16, 31
    b @L_801DFAF8
@L_801DFA9C:
    rlwinm r4, r25, 0, 16, 31
    mr r3, r26
    addi r24, r4, 0x1
    rlwinm r4, r24, 0, 16, 31
    bl fn_80123CD4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DFB04
    cmplwi r26, 0x0
    rlwinm r24, r24, 0, 16, 31
    beq @L_801DFAF4
    mr r3, r26
    mr r4, r25
    li r5, 0x0
    bl fn_8011F260
    mr r27, r3
    mr r3, r26
    mr r4, r24
    li r5, 0x0
    bl fn_8011F260
    mr r4, r3
    mr r3, r27
    bl fn_8011F5E0
@L_801DFAF4:
    addi r25, r25, 0x1
@L_801DFAF8:
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0x3
    blt @L_801DFA9C
@L_801DFB04:
    mr r3, r26
    rlwinm r4, r25, 0, 16, 31
    bl fn_80125314
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    mr r4, r29
    li r3, 0x39
    bl fn_80132A38
    li r3, 0x3b2c
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801DFB68
    li r4, 0x7
    b @L_801DFC14
@L_801DFB68:
    li r4, 0xc
    b @L_801DFC14
    li r3, 0x3b2d
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r4, 0xf
    b @L_801DFC14
    li r3, 0x3b2e
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801DFBB4
    li r4, 0x2
    b @L_801DFC14
@L_801DFBB4:
    li r4, 0xc
    b @L_801DFC14
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x44aa
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801DFC08
    li r4, 0x2
    b @L_801DFC14
@L_801DFC08:
    li r4, 0xc
    b @L_801DFC14
    li r28, 0x0
@L_801DFC14:
    cmpwi r28, 0x0
    bne @L_801DF7B0
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
