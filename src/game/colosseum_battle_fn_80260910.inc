    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stfd f29, 0x50(r1)
    psq_st f29, 0x58(r1), 0, 0
    stmw r18, 0x18(r1)
    mr r27, r3
    mr r28, r4
    lwz r3, 0x0(r3)
    mr r31, r5
    mr r30, r6
    li r4, 0x1
    bl fn_801DA4E8
    mr r3, r31
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x32
    mr r4, r0
    bl fn_80132A38
    li r3, 0x4401
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    cmplwi r31, 0x0
    bne @L_8026098C
    li r3, 0x0
    b @L_802609B0
@L_8026098C:
    mr r3, r31
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802609A4
    li r3, 0x0
    b @L_802609B0
@L_802609A4:
    mr r3, r31
    bl fn_8011F5C8
    bl fn_8011E778
@L_802609B0:
    cmplwi r3, 0x0
    bne @L_802609C0
    li r25, 0x0
    b @L_802609D0
@L_802609C0:
    bl fn_8011E15C
    rlwinm r25, r3, 0, 16, 31
    mr r3, r25
    bl fn_80166A28
@L_802609D0:
    lis r3, -32728
    addi r3, r3, -23416
    lwz r0, 0x4(r3)
    addi r23, r3, 0x4
    lhz r4, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_802609F4
    lwz r3, 0x4(r27)
    b @L_802609F8
@L_802609F4:
    lwz r3, 0x0(r27)
@L_802609F8:
    li r5, 0x4
    bl fn_801DA9E8
    bl fn_801DB088
    li r0, 0x0
    lis r3, -32728
    stw r0, 0x8(r27)
    addi r26, r3, -23416
    li r22, 0x1
    li r20, 0x0
    li r19, 0x0
    li r21, 0x0
    b @L_80260B58
@L_80260A28:
    cmpwi r28, 0x0
    beq @L_80260A68
    cmplwi r21, 0x78
    blt @L_80260A68
    cmpwi r19, 0x2
    blt @L_80260A68
    li r3, 0x1
    bl fn_800F7AF0
    mr r24, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r24
    rlwinm. r0, r0, 0, 22, 22
    beq @L_80260A68
    li r20, 0x1
    b @L_80260B60
@L_80260A68:
    lwz r4, 0x8(r27)
    rlwinm r0, r4, 3, 0, 28
    add r3, r26, r0
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_80260A88
    lwz r24, 0x4(r27)
    b @L_80260A8C
@L_80260A88:
    lwz r24, 0x0(r27)
@L_80260A8C:
    rlwinm r0, r4, 3, 0, 28
    lhzx r18, r26, r0
    bl fn_801DB088
    mr r3, r24
    mr r4, r18
    li r5, 0x4
    bl fn_801DA94C
    rlwinm r3, r3, 0, 24, 31
    neg r0, r3
    or r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    beq @L_80260B60
    cmpwi r19, 0x1
    beq @L_80260B00
    bge @L_80260B34
    cmpwi r19, 0x0
    bge @L_80260AD4
    b @L_80260B34
@L_80260AD4:
    mr r3, r25
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80260B34
    li r3, 0x3d3
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
    li r29, 0x0
    li r19, 0x1
    b @L_80260B34
@L_80260B00:
    li r3, 0x3d3
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80260B34
    bl fn_800D3088
    add r29, r29, r3
    cmpwi r29, 0x1e
    blt @L_80260B34
    li r3, 0x3d4
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
    li r19, 0x2
@L_80260B34:
    cmpwi r22, 0x0
    beq @L_80260B4C
    lfs f1, -20448(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r22, 0x0
@L_80260B4C:
    bl fn_800F0308
    bl fn_800D3088
    add r21, r21, r3
@L_80260B58:
    cmplwi r21, 0x23a
    blt @L_80260A28
@L_80260B60:
    lfs f1, -20448(r2)
    li r3, 0x5
    bl fn_801C41C8
    lis r3, -32728
    addi r19, r3, -23416
    b @L_80260BBC
@L_80260B78:
    lwz r4, 0x8(r27)
    rlwinm r0, r4, 3, 0, 28
    add r3, r19, r0
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_80260B98
    lwz r22, 0x4(r27)
    b @L_80260B9C
@L_80260B98:
    lwz r22, 0x0(r27)
@L_80260B9C:
    rlwinm r0, r4, 3, 0, 28
    lhzx r18, r19, r0
    bl fn_801DB088
    mr r3, r22
    mr r4, r18
    li r5, 0x4
    bl fn_801DA94C
    bl fn_800F0308
@L_80260BBC:
    li r3, 0x0
    bl fn_801C40F0
    extsb. r0, r3
    bne @L_80260B78
    lwz r5, 0x8(r27)
    rlwinm r0, r5, 3, 0, 28
    lwzx r0, r23, r0
    cmpwi r0, 0x0
    beq @L_80260BE8
    lwz r3, 0x4(r27)
    b @L_80260BEC
@L_80260BE8:
    lwz r3, 0x0(r27)
@L_80260BEC:
    lis r4, -32728
    rlwinm r0, r5, 3, 0, 28
    addi r4, r4, -23416
    li r5, 0x4
    lhzx r4, r4, r0
    bl fn_801DA8C4
    lfs f29, -20464(r2)
    lis r19, 0x4330
    lfd f31, -20456(r2)
    lfs f30, -20460(r2)
    b @L_80260C34
@L_80260C18:
    bl fn_800F0308
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r19, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f31
    fadds f29, f29, f0
@L_80260C34:
    fcmpo cr0, f29, f30
    blt @L_80260C18
    cmpwi r20, 0x0
    beq @L_80260D58
    li r3, 0x1
    bl fn_801065B8
    li r3, 0x3d4
    li r4, 0x32
    bl fn_801657F8
    lis r3, -32728
    lwzu r0, -23376(r3)
    li r4, 0x0
    cmplw r0, r21
    bge @L_80260C80
    lwz r0, 0x8(r3)
    li r4, 0x1
    cmplw r0, r21
    bge @L_80260C80
    li r4, 0x2
@L_80260C80:
    lis r3, -32728
    rlwinm r0, r4, 3, 0, 28
    addi r4, r3, -23376
    mr r3, r27
    add r4, r4, r0
    li r5, 0x0
    lwz r4, 0x4(r4)
    li r6, 0x8
    bl fn_802606D8
    cmplwi r31, 0x0
    bne @L_80260CB4
    li r3, 0x0
    b @L_80260CD8
@L_80260CB4:
    mr r3, r31
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80260CCC
    li r3, 0x0
    b @L_80260CD8
@L_80260CCC:
    mr r3, r31
    bl fn_8011F5C8
    bl fn_8011E778
@L_80260CD8:
    cmplwi r3, 0x0
    bne @L_80260CE8
    li r18, 0x0
    b @L_80260D00
@L_80260CE8:
    bl fn_8011E15C
    rlwinm r18, r3, 0, 16, 31
    mr r3, r18
    bl fn_80166A28
    b @L_80260D00
@L_80260CFC:
    bl fn_800F0308
@L_80260D00:
    mr r3, r18
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80260CFC
    mr r3, r31
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x43ff
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801065B8
    lfs f1, -20448(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x0
    b @L_80260E90
@L_80260D58:
    lwz r3, 0x0(r27)
    li r4, 0x0
    bl fn_801DA4E8
    lwz r3, 0x4(r27)
    li r4, 0x1
    bl fn_801DA4E8
    mr r3, r27
    li r4, 0x1
    li r5, 0x0
    li r6, 0x8
    bl fn_802606D8
    li r3, 0x1
    bl fn_801065B8
    li r3, 0x3d4
    li r4, 0x32
    bl fn_801657F8
    cmplwi r30, 0x0
    bne @L_80260DA8
    li r3, 0x0
    b @L_80260DCC
@L_80260DA8:
    mr r3, r30
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80260DC0
    li r3, 0x0
    b @L_80260DCC
@L_80260DC0:
    mr r3, r30
    bl fn_8011F5C8
    bl fn_8011E778
@L_80260DCC:
    cmplwi r3, 0x0
    bne @L_80260DDC
    li r18, 0x0
    b @L_80260DF4
@L_80260DDC:
    bl fn_8011E15C
    rlwinm r18, r3, 0, 16, 31
    mr r3, r18
    bl fn_80166A28
    b @L_80260DF4
@L_80260DF0:
    bl fn_800F0308
@L_80260DF4:
    mr r3, r18
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80260DF0
    mr r3, r31
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    mr r3, r30
    bl fn_8011F5C8
    mr r0, r3
    li r3, 0x4e
    rlwinm r4, r0, 0, 16, 31
    bl fn_80132A38
    li r3, 0x5d
    li r4, 0x3d2
    bl fn_80132A38
    li r3, 0x4400
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801065B8
    lfs f29, -20464(r2)
    lis r19, 0x4330
    lfd f30, -20456(r2)
    lfs f31, -20444(r2)
    b @L_80260E84
@L_80260E68:
    bl fn_800F0308
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r19, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f30
    fadds f29, f29, f0
@L_80260E84:
    fcmpo cr0, f29, f31
    blt @L_80260E68
    li r3, 0x1
@L_80260E90:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    psq_l f29, 0x58(r1), 0, 0
    lfd f29, 0x50(r1)
    lmw r18, 0x18(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
