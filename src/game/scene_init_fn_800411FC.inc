    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r30, r3
    mr r31, r4
    lwz r26, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmplwi r26, 0x0
    lwz r0, 0x0(r3)
    beq @L_800412D0
    beq @L_800412A8
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r28, r3, r0
    cmplwi r28, 0x8000
    blt @L_80041248
    clrlwi r28, r28, 18
    @L_80041248:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r27, r3
    mr r4, r28
    li r3, 0x0
    bl fn_8025FD34
    mr r28, r3
    mr r3, r26
    mr r4, r27
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r28
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_800412AC
    @L_800412A8:
    li r3, 0x0
    @L_800412AC:
    cmplwi r3, 0x0
    beq @L_800412C8
    bl fn_8011F5C8
    bl fn_8011E778
    bl fn_8011E760
    bl fn_800FA280
    b @L_800412D4
    @L_800412C8:
    li r3, 0x0
    b @L_800412D4
    @L_800412D0:
    li r3, 0x0
    @L_800412D4:
    cmplwi r3, 0x0
    bne @L_800412E4
    li r3, 0x1
    bl fn_800FA280
    @L_800412E4:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xce
    bl fn_800FB680
    lis r3, lbl_803A6818@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_803A6818@l
    lwz r5, lbl_8047A4E4(r13)
    lwz r3, 0x0(r3)
    addi r28, r4, lbl_802EF0A8@l
    slwi r0, r3, 1
    lhzx r0, r5, r0
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80041340
    li r0, 0x0
    b @L_80041344
    @L_80041340:
    li r0, 0x1
    @L_80041344:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_8004143C
    lwz r26, lbl_8047A4E0(r13)
    cmplwi r26, 0x0
    beq @L_800413D0
    slwi r0, r3, 1
    lhzx r29, r5, r0
    cmplwi r29, 0x8000
    blt @L_80041370
    clrlwi r29, r29, 18
    @L_80041370:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r29, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r29
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r29
    mr r29, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r26
    mr r4, r29
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r27
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_800413D4
    @L_800413D0:
    li r3, 0x0
    @L_800413D4:
    bl fn_8011F5C8
    bl fn_8011E778
    li r4, 0x0
    bl fn_8011E444
    clrlwi r3, r3, 24
    mr r29, r3
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r3, 0x5a5a(r28)
    li r6, 0xe7
    lbz r4, 0x8b(r30)
    subf r3, r5, r3
    lha r8, 0x52(r31)
    lha r7, 0x5a5c(r28)
    addi r3, r3, 0x4
    or r5, r4, r0
    subf r4, r8, r7
    bl fn_800FB680
    b @L_8004146C
    @L_8004143C:
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r3, 0x5a5a(r28)
    li r6, 0x3721
    lbz r4, 0x8b(r30)
    subf r3, r5, r3
    lha r8, 0x52(r31)
    lha r7, 0x5a5c(r28)
    addi r3, r3, 0x4
    or r5, r4, r0
    subf r4, r8, r7
    bl fn_800FB680
    @L_8004146C:
    lis r3, lbl_803A6818@ha
    lwz r4, lbl_8047A4E4(r13)
    addi r3, r3, lbl_803A6818@l
    lwz r3, 0x0(r3)
    slwi r0, r3, 1
    lhzx r0, r4, r0
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80041498
    li r0, 0x0
    b @L_8004149C
    @L_80041498:
    li r0, 0x1
    @L_8004149C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_800415AC
    lwz r26, lbl_8047A4E0(r13)
    cmplwi r26, 0x0
    beq @L_80041528
    slwi r0, r3, 1
    lhzx r28, r4, r0
    cmplwi r28, 0x8000
    blt @L_800414C8
    clrlwi r28, r28, 18
    @L_800414C8:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r28
    mr r28, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r26
    mr r4, r28
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r27
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8004152C
    @L_80041528:
    li r3, 0x0
    @L_8004152C:
    bl fn_8011F5C8
    bl fn_8011E778
    li r4, 0x1
    bl fn_8011E444
    clrlwi r3, r3, 24
    cmplwi r3, 0x0
    beq @L_800415DC
    clrlwi r0, r29, 16
    cmplw r3, r0
    beq @L_800415DC
    lis r4, lbl_802EF0A8@ha
    addi r4, r4, lbl_802EF0A8@l
    addis r26, r4, 0x2
    bl fn_8011CB6C
    bl fn_8011CB54
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r3, 0xb42(r26)
    li r6, 0xe7
    lbz r4, 0x8b(r30)
    subf r3, r5, r3
    lha r8, 0x52(r31)
    lha r7, 0xb44(r26)
    addi r3, r3, 0x4
    or r5, r4, r0
    subf r4, r8, r7
    bl fn_800FB680
    b @L_800415DC
    @L_800415AC:
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r3, 0x5a5a(r28)
    li r6, 0x3721
    lbz r4, 0x8b(r30)
    subf r3, r5, r3
    lha r8, 0x52(r31)
    lha r7, 0x5a5c(r28)
    addi r3, r3, 0x4
    or r5, r4, r0
    subf r4, r8, r7
    bl fn_800FB680
    @L_800415DC:
    lis r3, lbl_803A6818@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_803A6818@l
    lwz r5, lbl_8047A4E4(r13)
    lwz r3, 0x0(r3)
    addi r29, r4, lbl_802EF0A8@l
    slwi r0, r3, 1
    lhzx r0, r5, r0
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80041610
    li r0, 0x0
    b @L_80041614
    @L_80041610:
    li r0, 0x1
    @L_80041614:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_800417E0
    lwz r26, lbl_8047A4E0(r13)
    cmplwi r26, 0x0
    beq @L_800416D8
    beq @L_800416A4
    slwi r0, r3, 1
    lhzx r28, r5, r0
    cmplwi r28, 0x8000
    blt @L_80041644
    clrlwi r28, r28, 18
    @L_80041644:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r28
    mr r28, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r26
    mr r4, r28
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r27
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_800416A8
    @L_800416A4:
    li r3, 0x0
    @L_800416A8:
    cmplwi r3, 0x0
    beq @L_800416D0
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_800416C8
    bl fn_8011E1A4
    b @L_800416DC
    @L_800416C8:
    li r3, 0x0
    b @L_800416DC
    @L_800416D0:
    li r3, 0x0
    b @L_800416DC
    @L_800416D8:
    li r3, 0x0
    @L_800416DC:
    li r8, 0x9
    lis r4, lbl_803A67E8@ha
    slwi r5, r8, 1
    li r7, 0x0
    addi r6, r4, lbl_803A67E8@l
    li r8, 0x8
    slwi r0, r8, 1
    sthx r7, r6, r5
    li r4, 0x27
    li r8, 0x7
    sthx r4, r6, r0
    slwi r5, r8, 1
    clrlwi r7, r3, 16
    li r9, 0x3
    sthx r4, r6, r5
    lis r3, 0xcccd
    subi r4, r3, 0x3333
    @L_80041720:
    mulhwu r3, r4, r7
    cmpwi r9, 0x0
    mr r0, r3
    srwi r3, r3, 3
    mulli r3, r3, 0xa
    subf r3, r3, r7
    srwi r7, r0, 3
    bgt @L_8004174C
    cmplwi r3, 0x0
    beq @L_80041788
    b @L_80041750
    @L_8004174C:
    subi r9, r9, 0x1
    @L_80041750:
    addi r0, r3, 0x30
    cmpwi r9, 0x1
    clrlwi r0, r0, 16
    subi r5, r5, 0x2
    sthx r0, r6, r5
    subi r8, r8, 0x1
    bne @L_80041720
    lis r3, lbl_803A67E8@ha
    subi r5, r5, 0x2
    addi r3, r3, lbl_803A67E8@l
    li r0, 0x27
    sthx r0, r3, r5
    subi r8, r8, 0x1
    b @L_80041720
    @L_80041788:
    lis r3, lbl_803A67E8@ha
    slwi r4, r8, 1
    addi r0, r3, lbl_803A67E8@l
    li r3, 0x37
    add r4, r0, r4
    bl fn_80132A38
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r4, 0x5a76(r29)
    li r8, 0xcf
    lbz r3, 0x8b(r30)
    lwz r6, lbl_8047A4F0(r13)
    subf r5, r5, r4
    lha r4, 0x52(r31)
    or r7, r3, r0
    lha r0, 0x5a78(r29)
    add r3, r6, r5
    lha r5, 0x5a7a(r29)
    lha r6, 0x5a7c(r29)
    subf r4, r4, r0
    bl fn_800FB8C8
    b @L_8004181C
    @L_800417E0:
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r4, 0x5a76(r29)
    li r8, 0x371b
    lbz r3, 0x8b(r30)
    lwz r6, lbl_8047A4F0(r13)
    subf r5, r5, r4
    lha r4, 0x52(r31)
    or r7, r3, r0
    lha r0, 0x5a78(r29)
    add r3, r6, r5
    lha r5, 0x5a7a(r29)
    lha r6, 0x5a7c(r29)
    subf r4, r4, r0
    bl fn_800FBB34
    @L_8004181C:
    lis r3, lbl_803A6818@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_803A6818@l
    lwz r5, lbl_8047A4E4(r13)
    lwz r3, 0x0(r3)
    addi r29, r4, lbl_802EF0A8@l
    slwi r0, r3, 1
    lhzx r0, r5, r0
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80041850
    li r0, 0x0
    b @L_80041854
    @L_80041850:
    li r0, 0x1
    @L_80041854:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_80041998
    lwz r26, lbl_8047A4E0(r13)
    cmplwi r26, 0x0
    beq @L_80041918
    beq @L_800418E4
    slwi r0, r3, 1
    lhzx r28, r5, r0
    cmplwi r28, 0x8000
    blt @L_80041884
    clrlwi r28, r28, 18
    @L_80041884:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r28
    mr r28, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r26
    mr r4, r28
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r27
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_800418E8
    @L_800418E4:
    li r3, 0x0
    @L_800418E8:
    cmplwi r3, 0x0
    beq @L_80041910
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_80041908
    bl fn_8011E18C
    b @L_8004191C
    @L_80041908:
    li r3, 0x0
    b @L_8004191C
    @L_80041910:
    li r3, 0x0
    b @L_8004191C
    @L_80041918:
    li r3, 0x0
    @L_8004191C:
    lis r4, 0xcccd
    clrlwi r26, r3, 16
    subi r0, r4, 0x3333
    li r3, 0x34
    mulhwu r0, r0, r26
    srwi r4, r0, 3
    bl fn_80132A38
    lis r4, 0xcccd
    li r3, 0x35
    subi r0, r4, 0x3333
    mulhwu r0, r0, r26
    srwi r0, r0, 3
    mulli r0, r0, 0xa
    subf r4, r0, r26
    bl fn_80132A38
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r4, 0x5a92(r29)
    li r8, 0x189c
    lbz r3, 0x8b(r30)
    lwz r6, lbl_8047A4F0(r13)
    subf r5, r5, r4
    lha r4, 0x52(r31)
    or r7, r3, r0
    lha r0, 0x5a94(r29)
    add r3, r6, r5
    lha r5, 0x5a96(r29)
    lha r6, 0x5a98(r29)
    subf r4, r4, r0
    bl fn_800FBB34
    b @L_800419D4
    @L_80041998:
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r4, 0x5a92(r29)
    li r8, 0x371a
    lbz r3, 0x8b(r30)
    lwz r6, lbl_8047A4F0(r13)
    subf r5, r5, r4
    lha r4, 0x52(r31)
    or r7, r3, r0
    lha r0, 0x5a94(r29)
    add r3, r6, r5
    lha r5, 0x5a96(r29)
    lha r6, 0x5a98(r29)
    subf r4, r4, r0
    bl fn_800FBB34
    @L_800419D4:
    lis r3, lbl_803A6818@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_803A6818@l
    lwz r5, lbl_8047A4E4(r13)
    lwz r3, 0x0(r3)
    addi r29, r4, lbl_802EF0A8@l
    slwi r0, r3, 1
    lhzx r0, r5, r0
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80041A08
    li r0, 0x0
    b @L_80041A0C
    @L_80041A08:
    li r0, 0x1
    @L_80041A0C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_80041B00
    lwz r26, lbl_8047A4E0(r13)
    cmplwi r26, 0x0
    beq @L_80041AB0
    beq @L_80041A9C
    slwi r0, r3, 1
    lhzx r28, r5, r0
    cmplwi r28, 0x8000
    blt @L_80041A3C
    clrlwi r28, r28, 18
    @L_80041A3C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r28
    mr r28, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r26
    mr r4, r28
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r27
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_80041AA0
    @L_80041A9C:
    li r3, 0x0
    @L_80041AA0:
    bl fn_8011F5C8
    bl fn_8011E778
    bl fn_8011E1BC
    b @L_80041AB4
    @L_80041AB0:
    li r3, 0x0
    @L_80041AB4:
    mr r4, r3
    li r3, 0x31
    bl fn_80132A38
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r4, 0x5a22(r29)
    li r8, 0x371e
    lbz r3, 0x8b(r30)
    lwz r6, lbl_8047A4F0(r13)
    subf r5, r5, r4
    lha r4, 0x52(r31)
    or r7, r3, r0
    lha r0, 0x5a24(r29)
    add r3, r6, r5
    lha r5, 0x5a26(r29)
    lha r6, 0x5a28(r29)
    subf r4, r4, r0
    bl fn_800FBB34
    b @L_80041B48
    @L_80041B00:
    li r3, 0x31
    li r4, 0x371c
    bl fn_80132A38
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r4, 0x5a22(r29)
    li r8, 0x371e
    lbz r3, 0x8b(r30)
    lwz r6, lbl_8047A4F0(r13)
    subf r5, r5, r4
    lha r4, 0x52(r31)
    or r7, r3, r0
    lha r0, 0x5a24(r29)
    add r3, r6, r5
    lha r5, 0x5a26(r29)
    lha r6, 0x5a28(r29)
    subf r4, r4, r0
    bl fn_800FBB34
    @L_80041B48:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
