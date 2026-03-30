    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r25, r3
    mr r26, r4
    li r28, 0x0
    li r24, 0x0
@L_800767D8:
    mr r3, r25
    mr r4, r26
    mr r5, r24
    bl fn_80076F2C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800767FC
    li r0, 0x0
    b @L_8007680C
@L_800767FC:
    addi r24, r24, 0x1
    cmpwi r24, 0x4
    blt @L_800767D8
    li r0, 0x1
@L_8007680C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076820
    li r3, 0x0
    b @L_80076A78
@L_80076820:
    li r29, 0x0
@L_80076824:
    mr r3, r25
    rlwinm r4, r29, 0, 16, 31
    bl fn_8012AC08
    mr r27, r3
    cmplwi r27, 0x0
    beq @L_80076A60
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80076A60
    cntlzw r0, r27
    li r30, 0x0
    rlwinm r31, r0, 27, 5, 31
@L_80076858:
    cmpwi r31, 0x0
    li r24, 0x0
    bne @L_80076880
    mr r3, r27
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076884
@L_80076880:
    li r24, 0x1
@L_80076884:
    cmpwi r24, 0x0
    beq @L_80076894
    li r0, 0x1
    b @L_80076A24
@L_80076894:
    cmpwi r30, 0x1
    beq @L_800768E0
    bge @L_800768AC
    cmpwi r30, 0x0
    bge @L_800768B8
    b @L_80076A08
@L_800768AC:
    cmpwi r30, 0x3
    bge @L_80076A08
    b @L_80076908
@L_800768B8:
    mr r3, r27
    bl fn_8011F4A8
    rlwinm r5, r3, 0, 24, 31
    lha r0, 0x0(r26)
    srawi r4, r5, 31
    rlwinm r3, r0, 1, 31, 31
    subfc r0, r0, r5
    adde r0, r4, r3
    rlwinm r0, r0, 0, 24, 31
    b @L_80076A24
@L_800768E0:
    mr r3, r27
    bl fn_8011F4A8
    lha r0, 0x2(r26)
    rlwinm r5, r3, 0, 24, 31
    rlwinm r3, r5, 1, 31, 31
    srawi r4, r0, 31
    subfc r0, r5, r0
    adde r0, r4, r3
    rlwinm r0, r0, 0, 24, 31
    b @L_80076A24
@L_80076908:
    mr r3, r27
    bl fn_8011F1A0
    mr r23, r3
    bl fn_8006B420
    rlwinm r0, r23, 0, 16, 31
    mr r24, r3
    cmpwi r0, 0xaf
    beq @L_80076940
    bge @L_80076948
    cmpwi r0, 0x0
    beq @L_80076938
    b @L_80076948
@L_80076938:
    li r3, 0x1
    b @L_80076950
@L_80076940:
    li r3, 0x0
    b @L_80076950
@L_80076948:
    mr r3, r23
    bl fn_80142984
@L_80076950:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076964
    li r0, 0x0
    b @L_80076A24
@L_80076964:
    lwz r0, 0x8(r24)
    cmpwi r0, 0x1
    beq @L_80076994
    bge @L_80076980
    cmpwi r0, 0x0
    bge @L_8007698C
    b @L_80076A00
@L_80076980:
    cmpwi r0, 0x3
    bge @L_80076A00
    b @L_800769A8
@L_8007698C:
    li r0, 0x1
    b @L_80076A24
@L_80076994:
    rlwinm r0, r23, 0, 16, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    b @L_80076A24
@L_800769A8:
    lis r3, -32721
    lwz r0, -32504(r13)
    addi r5, r3, -7080
    li r4, 0x0
    rlwinm r3, r23, 0, 16, 31
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800769F8
@L_800769C8:
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_800769EC
    addi r0, r4, 0x18
    lbzx r0, r24, r0
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    b @L_80076A24
@L_800769EC:
    addi r5, r5, 0x2
    addi r4, r4, 0x1
    bdnz @L_800769C8
@L_800769F8:
    li r0, 0x1
    b @L_80076A24
@L_80076A00:
    li r0, 0x0
    b @L_80076A24
@L_80076A08:
    lis r3, -32729
    lis r5, -32729
    addi r3, r3, -30136
    li r4, 0xfb
    addi r5, r5, -30120
    bl fn_80196E10
    li r0, 0x0
@L_80076A24:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076A38
    li r0, 0x0
    b @L_80076A48
@L_80076A38:
    addi r30, r30, 0x1
    cmpwi r30, 0x3
    blt @L_80076858
    li r0, 0x1
@L_80076A48:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076A5C
    li r3, 0x0
    b @L_80076A78
@L_80076A5C:
    addi r28, r28, 0x1
@L_80076A60:
    addi r29, r29, 0x1
    cmpwi r29, 0x6
    blt @L_80076824
    neg r0, r28
    andc r0, r0, r28
    rlwinm r3, r0, 1, 31, 31
@L_80076A78:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
