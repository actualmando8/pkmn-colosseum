    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r27, r3
    bl fn_80115A80
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80115704
    mr r31, r3
    li r29, 0x0
    b @L_801134C8
@L_8011327C:
    mr r3, r29
    bl fn_8011711C
    mr r28, r3
    bl fn_80117070
    bl fn_8018F6B4
    mr r27, r3
    bl fn_800FF548
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801132BC
    mr r3, r30
    mr r4, r29
    mr r5, r27
    bl fn_8018E050
    mr r27, r3
    b @L_801132CC
@L_801132BC:
    mr r3, r30
    mr r4, r29
    bl fn_8018D998
    mr r27, r3
@L_801132CC:
    cmplwi r27, 0x0
    bne @L_801132F0
    lis r3, lbl_802720CC@ha
    lis r4, lbl_8035B888@ha
    addi r3, r3, lbl_802720CC@l
    addi r4, r4, lbl_8035B888@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801134C4
@L_801132F0:
    bl fn_800FF548
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_801134C4
    mr r3, r30
    mr r4, r29
    bl fn_80183958
    mr r0, r3
    mr r3, r30
    mr r5, r0
    mr r4, r29
    mr r6, r27
    li r7, 0x0
    bl fn_801837D8
    mr r3, r30
    mr r4, r29
    li r5, 0x4
    bl fn_8018C7C8
    mr r3, r30
    mr r4, r29
    li r5, 0x8
    bl fn_8018C7C8
    mr r3, r28
    addi r4, r1, 0x14
    bl fn_80116FE0
    mr r3, r30
    mr r4, r29
    addi r5, r1, 0x14
    bl fn_8018C0A8
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_80116F68
    mr r3, r30
    mr r4, r29
    addi r5, r1, 0x8
    bl fn_8018BF24
    mr r3, r30
    mr r4, r29
    bl fn_8018CB5C
    mr r3, r28
    bl fn_80117054
    mr r5, r3
    mr r3, r30
    mr r4, r29
    bl fn_8018C1E8
    mr r3, r28
    bl fn_80117038
    mr r5, r3
    mr r3, r30
    mr r4, r29
    bl fn_8018CA20
    mr r3, r28
    bl fn_80116F4C
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x2
    beq @L_801133E8
    bge @L_801133DC
    cmpwi r0, 0x0
    b @L_80113410
@L_801133DC:
    cmpwi r0, 0x4
    bge @L_80113410
    b @L_801133FC
@L_801133E8:
    lfs f1, -26392(r2)
    mr r3, r30
    mr r4, r29
    bl fn_80183B44
    b @L_80113410
@L_801133FC:
    lfs f1, -26388(r2)
    mr r3, r30
    lfs f2, -26384(r2)
    mr r4, r29
    bl fn_801839A0
@L_80113410:
    mr r3, r28
    bl fn_80116F30
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x1
    beq @L_8011343C
    bge @L_80113430
    cmpwi r0, 0x0
    b @L_80113460
@L_80113430:
    cmpwi r0, 0x3
    bge @L_80113460
    b @L_80113450
@L_8011343C:
    mr r3, r30
    mr r4, r29
    li r5, 0x10
    bl fn_8018C7C8
    b @L_80113460
@L_80113450:
    mr r3, r30
    mr r4, r29
    li r5, 0x20
    bl fn_8018C7C8
@L_80113460:
    mr r3, r28
    bl fn_80116F14
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x1
    beq @L_80113498
    bge @L_801134A8
    cmpwi r0, 0x0
    bge @L_80113484
    b @L_801134A8
@L_80113484:
    mr r3, r30
    mr r4, r29
    li r5, 0x40
    bl fn_8018C69C
    b @L_801134A8
@L_80113498:
    mr r3, r30
    mr r4, r29
    li r5, 0x40
    bl fn_8018C7C8
@L_801134A8:
    lbz r0, 0x0(r28)
    rlwinm r0, r0, 27, 31, 31
    cmplwi r0, 0x0
    beq @L_801134C4
    mr r3, r30
    mr r4, r29
    bl fn_80188F78
@L_801134C4:
    addi r29, r29, 0x1
@L_801134C8:
    cmplw r29, r31
    blt @L_8011327C
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

