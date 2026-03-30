    nofralloc
    stwu r1, -0x150(r1)
    mflr r0
    stw r0, 0x154(r1)
    stw r31, 0x14c(r1)
    stw r30, 0x148(r1)
    stw r29, 0x144(r1)
    stw r28, 0x140(r1)
    mr r31, r3
    mr r28, r4
    cmplwi r31, 0x0
    lis r4, -32729
    addi r29, r4, -30400
    li r30, 0x0
    beq @L_800763E8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_800763EC
@L_800763E8:
    li r30, 0x1
@L_800763EC:
    cmpwi r30, 0x0
    beq @L_800763FC
    li r3, 0x1
    b @L_80076798
@L_800763FC:
    cmpwi r28, 0x3
    beq @L_800766B0
    bge @L_80076420
    cmpwi r28, 0x1
    beq @L_80076678
    bge @L_80076694
    cmpwi r28, 0x0
    bge @L_80076430
    b @L_80076784
@L_80076420:
    cmpwi r28, 0x5
    beq @L_80076748
    bge @L_80076784
    b @L_800766EC
@L_80076430:
    mr r3, r31
    li r4, 0x2
    bl fn_80076398
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076450
    li r3, 0x1
    b @L_80076798
@L_80076450:
    mr r3, r31
    bl fn_8011F5C8
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0x19a
    beq @L_80076474
    bge @L_80076490
    cmpwi r0, 0x97
    beq @L_80076474
    b @L_80076490
@L_80076474:
    mr r3, r31
    bl fn_8011E7A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076490
    li r0, 0x0
    b @L_80076494
@L_80076490:
    li r0, 0x1
@L_80076494:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800764A8
    li r3, 0x0
    b @L_80076798
@L_800764A8:
    mr r4, r31
    addi r3, r1, 0x8
    bl fn_8011F5FC
    addi r3, r1, 0x8
    bl fn_8012546C
    mr r3, r31
    bl fn_8011F15C
    rlwinm r30, r3, 0, 16, 31
    addi r3, r1, 0x8
    bl fn_8011F15C
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r30
    blt @L_8007657C
    mr r3, r31
    bl fn_8011F130
    rlwinm r30, r3, 0, 16, 31
    addi r3, r1, 0x8
    bl fn_8011F130
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r30
    blt @L_8007657C
    mr r3, r31
    bl fn_8011F104
    rlwinm r30, r3, 0, 16, 31
    addi r3, r1, 0x8
    bl fn_8011F104
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r30
    blt @L_8007657C
    mr r3, r31
    bl fn_8011F0D8
    rlwinm r30, r3, 0, 16, 31
    addi r3, r1, 0x8
    bl fn_8011F0D8
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r30
    blt @L_8007657C
    mr r3, r31
    bl fn_8011F0AC
    rlwinm r30, r3, 0, 16, 31
    addi r3, r1, 0x8
    bl fn_8011F0AC
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r30
    blt @L_8007657C
    mr r3, r31
    bl fn_8011F080
    rlwinm r30, r3, 0, 16, 31
    addi r3, r1, 0x8
    bl fn_8011F080
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r30
    bge @L_80076584
@L_8007657C:
    li r3, 0x0
    b @L_80076798
@L_80076584:
    mr r3, r31
    bl fn_8011F054
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    bl fn_8011F028
    rlwinm r0, r3, 0, 16, 31
    mr r3, r31
    add r29, r29, r0
    bl fn_8011EFFC
    rlwinm r0, r3, 0, 16, 31
    mr r3, r31
    add r29, r29, r0
    bl fn_8011EFD0
    rlwinm r0, r3, 0, 16, 31
    mr r3, r31
    add r29, r29, r0
    bl fn_8011EFA4
    rlwinm r0, r3, 0, 16, 31
    mr r3, r31
    add r29, r29, r0
    bl fn_8011EF78
    rlwinm r0, r3, 0, 16, 31
    add r29, r29, r0
    cmplwi r29, 0x1fe
    ble @L_800765F0
    li r3, 0x0
    b @L_80076798
@L_800765F0:
    mr r3, r31
    bl fn_8011F054
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xff
    bgt @L_80076668
    mr r3, r31
    bl fn_8011F028
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xff
    bgt @L_80076668
    mr r3, r31
    bl fn_8011EFFC
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xff
    bgt @L_80076668
    mr r3, r31
    bl fn_8011EFD0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xff
    bgt @L_80076668
    mr r3, r31
    bl fn_8011EFA4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xff
    bgt @L_80076668
    mr r3, r31
    bl fn_8011EF78
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xff
    ble @L_80076670
@L_80076668:
    li r3, 0x0
    b @L_80076798
@L_80076670:
    li r3, 0x1
    b @L_80076798
@L_80076678:
    mr r3, r31
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    b @L_80076798
@L_80076694:
    mr r3, r31
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    b @L_80076798
@L_800766B0:
    mr r3, r31
    bl fn_8011F1A0
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0xaf
    beq @L_800766DC
    bge @L_800766E4
    cmpwi r0, 0x0
    beq @L_800766D4
    b @L_800766E4
@L_800766D4:
    li r3, 0x1
    b @L_80076798
@L_800766DC:
    li r3, 0x0
    b @L_80076798
@L_800766E4:
    bl fn_80142984
    b @L_80076798
@L_800766EC:
    li r29, 0x0
@L_800766F0:
    mr r3, r31
    rlwinm r6, r29, 0, 16, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    cmplwi r28, 0x0
    beq @L_80076734
    li r3, 0x0
    bl fn_8011CA34
    mr r30, r3
    mr r3, r28
    bl fn_8011CA34
    cmplw r3, r30
    bne @L_80076734
    li r3, 0x0
    b @L_80076798
@L_80076734:
    addi r29, r29, 0x1
    cmpwi r29, 0x4
    blt @L_800766F0
    li r3, 0x1
    b @L_80076798
@L_80076748:
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80076778
    addi r3, r29, 0x108
    addi r5, r29, 0x14c
    li r4, 0x25e
    bl fn_80196E10
@L_80076778:
    mr r3, r31
    bl fn_80123FBC
    b @L_80076798
@L_80076784:
    addi r3, r29, 0x108
    addi r5, r29, 0x118
    li r4, 0x274
    bl fn_80196E10
    li r3, 0x0
@L_80076798:
    lwz r0, 0x154(r1)
    lwz r31, 0x14c(r1)
    lwz r30, 0x148(r1)
    lwz r29, 0x144(r1)
    lwz r28, 0x140(r1)
    mtlr r0
    addi r1, r1, 0x150
    blr
