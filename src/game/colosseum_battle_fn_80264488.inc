    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stmw r16, 0x30(r1)
    li r6, 0x0
    mr r28, r3
    stw r6, 0x20(r1)
    mr r29, r4
    mr r30, r5
    addi r5, r1, 0x1c
    addi r4, r1, 0x18
    addi r3, r1, 0x14
    li r7, 0x0
    b @L_802644E4
@L_802644D0:
    rlwinm r0, r7, 0, 16, 31
    addi r7, r7, 0x1
    stbx r6, r5, r0
    stbx r6, r4, r0
    stbx r6, r3, r0
@L_802644E4:
    rlwinm r0, r7, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_802644D0
    li r17, 0x0
    b @L_80264574
@L_802644F8:
    mr r3, r28
    mr r6, r17
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr. r18, r3
    beq @L_80264570
    mr r3, r28
    mr r4, r18
    bl fn_801F85B0
    mr r0, r3
    mr r3, r28
    rlwinm r6, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    bl fn_80206608
    rlwinm r16, r17, 0, 16, 31
    addi r4, r1, 0x1c
    stbx r3, r4, r16
    mr r3, r18
    li r4, 0x8
    bl fn_802026E4
    addi r5, r1, 0x18
    li r4, 0x7
    stbx r3, r5, r16
    mr r3, r18
    bl fn_802026E4
    addi r4, r1, 0x14
    stbx r3, r4, r16
@L_80264570:
    addi r17, r17, 0x1
@L_80264574:
    rlwinm r0, r17, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_802644F8
    mr r3, r28
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_802645A4
    li r3, 0x0
    b @L_80264AB8
@L_802645A4:
    mr r3, r29
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r28
    mr r4, r0
    bl fn_801F8638
    extsh r0, r3
    lis r4, -32730
    lis r3, -32730
    lis r6, -32730
    addi r23, r4, 8116
    lis r5, -32730
    addi r22, r3, 7356
    lis r4, -32730
    lis r3, -32730
    stw r0, 0x20(r1)
    lfd f30, -20432(r2)
    addi r24, r6, 7148
    lfs f31, -20436(r2)
    addi r25, r5, 7928
    addi r26, r4, 7564
    addi r27, r3, 8324
    lis r16, 0x4330
@L_8026460C:
    mr r4, r22
    li r3, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801F2B5C
    mr r4, r23
    li r3, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r3, 0x0
    bl fn_801F1700
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8026464C
    bl fn_8000DDBC
@L_8026464C:
    li r3, 0x0
    bl fn_801F1758
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80264664
    bl fn_8000DD30
@L_80264664:
    mr r4, r22
    li r3, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801F2B5C
    li r17, 0x1
@L_8026467C:
    stb r17, 0x11(r1)
    mr r4, r24
    addi r5, r1, 0x11
    li r3, 0x0
    li r6, 0x0
    bl fn_801F2B5C
    lbz r0, 0x11(r1)
    cmplwi r0, 0x1
    beq @L_802646A8
    bl fn_800F0308
    b @L_8026467C
@L_802646A8:
    mr r4, r23
    li r3, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
@L_802646BC:
    mr r4, r25
    li r3, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802646E4
    bl fn_800F0308
    b @L_802646BC
@L_802646E4:
    li r3, 0x0
    bl fn_801F1700
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8026470C
@L_802646F8:
    bl fn_8000DD98
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8026470C
    bl fn_800F0308
    b @L_802646F8
@L_8026470C:
    li r3, 0x0
    bl fn_801F1758
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80264734
@L_80264720:
    bl fn_8000DD0C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80264734
    bl fn_800F0308
    b @L_80264720
@L_80264734:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r16, 0x28(r1)
    stw r0, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f30
    fdivs f1, f31, f0
    bl fn_8010206C
    mr r5, r28
    addi r4, r1, 0x20
    li r3, 0x1
    bl fn_80018F88
    mr r0, r3
    li r3, 0xa
    mr r31, r0
    bl fn_801EFFC4
    bl fn_80019064
    rlwinm. r0, r31, 0, 16, 31
    rlwinm r21, r3, 0, 24, 31
    beq @L_80264854
    mr r4, r31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80264854
    li r19, 0x0
    b @L_80264848
@L_802647AC:
    mr r3, r28
    mr r6, r19
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr. r18, r3
    beq @L_80264844
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r17, r3
    beq @L_80264844
    rlwinm r20, r19, 0, 16, 31
    addi r3, r1, 0x18
    lbzx r0, r3, r20
    cmplwi r0, 0x1
    bne @L_80264814
    mr r3, r18
    li r4, 0x8
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80264814
    mr r3, r17
    li r4, 0x1
    bl fn_801DA36C
@L_80264814:
    addi r3, r1, 0x14
    lbzx r0, r3, r20
    cmplwi r0, 0x1
    bne @L_80264844
    mr r3, r18
    li r4, 0x7
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80264844
    mr r3, r17
    li r4, 0x2
    bl fn_801DA36C
@L_80264844:
    addi r19, r19, 0x1
@L_80264848:
    rlwinm r0, r19, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_802647AC
@L_80264854:
    mr r4, r26
    li r3, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801F2B5C
    li r0, 0x1
    mr r4, r27
    stb r0, 0x10(r1)
    addi r5, r1, 0x10
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r3, 0x0
    bl fn_801F1700
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8026489C
    bl fn_8000DDE8
@L_8026489C:
    li r3, 0x0
    bl fn_801F1758
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802648B4
    bl fn_8000DD5C
@L_802648B4:
    cmplwi r29, 0x0
    beq @L_80264954
    li r3, 0x0
    bl fn_801F18DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80264954
    mr r4, r29
    mr r5, r30
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_802648F0
    li r3, 0x0
    b @L_8026494C
@L_802648F0:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r17, r3, 0, 16, 31
    mr r3, r29
    mr r4, r30
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80264920
    li r3, 0x0
    b @L_8026494C
@L_80264920:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80264938
    li r3, 0x0
    b @L_8026494C
@L_80264938:
    mr r4, r17
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
@L_8026494C:
    li r4, 0x1
    bl fn_80011D9C
@L_80264954:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r16, 0x28(r1)
    stw r0, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f30
    fdivs f1, f31, f0
    bl fn_80102038
    li r3, 0xa
    bl fn_801EFFC4
    rlwinm. r0, r31, 0, 16, 31
    bne @L_8026498C
    li r3, 0x0
    b @L_80264AB8
@L_8026498C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802649EC
    mr r3, r28
    bl fn_801F7EF0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8026460C
    mr r3, r29
    mr r4, r31
    mr r5, r30
    bl fn_80264ADC
    cmplwi r3, 0x0
    beq @L_8026460C
    mr r4, r30
    bl fn_801F0134
    li r4, 0x0
    mr r9, r3
    b @L_80264A8C
@L_802649EC:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80264A80
    bl fn_8022FF90
    li r16, 0x0
    b @L_80264A74
@L_80264A18:
    mr r3, r28
    mr r6, r16
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr. r17, r3
    beq @L_80264A70
    rlwinm r0, r16, 0, 16, 31
    addi r4, r1, 0x1c
    lbzx r0, r4, r0
    cmplwi r0, 0x0
    bne @L_80264A70
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80264A70
    mr r3, r17
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
@L_80264A70:
    addi r16, r16, 0x1
@L_80264A74:
    rlwinm r0, r16, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_80264A18
@L_80264A80:
    lwz r0, 0x20(r1)
    li r4, 0x1
    rlwinm r9, r0, 0, 16, 31
@L_80264A8C:
    lis r3, -32713
    stw r4, 0x8(r1)
    addi r7, r3, 23920
    mr r10, r21
    mr r3, r29
    rlwinm r8, r31, 0, 16, 31
    li r4, 0x0
    li r5, 0x12
    li r6, 0x0
    bl fn_80204CE0
    li r3, 0x1
@L_80264AB8:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    lmw r16, 0x30(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
