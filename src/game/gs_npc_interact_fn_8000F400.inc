    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r28, r3
    mr r27, r4
    li r26, -0x1
    li r4, 0x0
    bl fn_801040D0
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8000F754
    bl fn_80103FE4
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_8000F754
    mr r3, r28
    li r4, 0x1
    bl fn_801040D0
    mr r31, r3
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    blt @L_8000F754
    lwz r3, 0x40(r30)
    bl fn_80205B8C
    cmplwi r3, 0x0
    beq @L_8000F754
    lwz r0, 0x0(r31)
    li r4, 0x0
    li r5, 0x7f
    clrlwi r6, r0, 16
    bl fn_8012640C
    clrlwi r25, r3, 16
    mr r3, r28
    mr r4, r27
    bl fn_8001D834
    lha r4, 0x6(r27)
    mr r29, r3
    subi r0, r4, 0x11f4
    cmplwi r0, 0xb
    bgt @L_8000F754
    lis r3, jumptable_802E4CA8@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802E4CA8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    cmpwi r4, 0x11f6
    beq @L_8000F4F0
    bge @L_8000F4D8
    cmpwi r4, 0x11f4
    beq @L_8000F4E8
    bge @L_8000F4F8
    b @L_8000F4FC
    @L_8000F4D8:
    cmpwi r4, 0x11f8
    bge @L_8000F4FC
    li r26, 0x0
    b @L_8000F4FC
    @L_8000F4E8:
    li r26, 0x1
    b @L_8000F4FC
    @L_8000F4F0:
    li r26, 0x2
    b @L_8000F4FC
    @L_8000F4F8:
    li r26, 0x3
    @L_8000F4FC:
    lwz r0, 0x0(r31)
    cmpw r26, r0
    bne @L_8000F518
    mr r3, r27
    li r4, 0x1
    bl fn_80109220
    b @L_8000F754
    @L_8000F518:
    mr r3, r27
    li r4, 0x0
    bl fn_80109220
    b @L_8000F754
    lwz r0, 0x0(r31)
    mr r5, r28
    li r3, 0x0
    li r4, 0x0
    mulli r0, r0, 0xc
    li r7, 0x0
    add r6, r30, r0
    lhz r6, 0xc(r6)
    bl fn_801040F0
    b @L_8000F754
    mr r4, r25
    li r3, 0x0
    li r5, 0x22
    li r6, 0x0
    bl fn_8011BEB4
    lha r5, 0x54(r27)
    mr r8, r3
    lha r6, 0x56(r27)
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    bl fn_800FBB34
    b @L_8000F754
    mr r4, r25
    li r3, 0x0
    li r5, 0x6
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x1
    bgt @L_8000F5C0
    lha r5, 0x54(r27)
    mr r7, r29
    lha r6, 0x56(r27)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be2
    bl fn_800FB8C8
    b @L_8000F754
    @L_8000F5C0:
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r27)
    mr r7, r29
    lha r6, 0x56(r27)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FB8C8
    b @L_8000F754
    mr r4, r25
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x1
    bgt @L_8000F628
    lha r5, 0x54(r27)
    mr r7, r29
    lha r6, 0x56(r27)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x2be2
    bl fn_800FB8C8
    b @L_8000F754
    @L_8000F628:
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r27)
    mr r7, r29
    lha r6, 0x56(r27)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FB8C8
    b @L_8000F754
    lwz r0, 0x0(r31)
    li r3, 0x37
    mulli r0, r0, 0xc
    add r4, r30, r0
    lwz r4, 0x4(r4)
    bl fn_80132A38
    mr r5, r29
    li r3, 0x0
    li r4, 0x0
    li r6, 0xcf
    bl fn_800FB680
    b @L_8000F754
    li r3, 0x1a4
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r27)
    extsh r28, r3
    mr r5, r29
    subf r0, r28, r0
    li r3, 0x0
    extsh r26, r0
    li r4, 0x0
    li r6, 0x1a4
    bl fn_800FB680
    li r3, 0x197
    bl fn_800FA444
    srwi r0, r3, 16
    mr r5, r29
    extsh r0, r0
    li r4, 0x0
    subf r3, r0, r26
    li r6, 0x197
    srwi r0, r3, 31
    add r0, r0, r3
    srawi r0, r0, 1
    extsh r25, r0
    add r3, r28, r25
    bl fn_800FB680
    lwz r0, 0x0(r31)
    li r3, 0x34
    mulli r0, r0, 0xc
    add r4, r30, r0
    lbz r4, 0xf(r4)
    bl fn_80132A38
    lha r6, 0x56(r27)
    mr r3, r28
    mr r5, r25
    mr r7, r29
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    lwz r0, 0x0(r31)
    li r3, 0x34
    mulli r0, r0, 0xc
    add r4, r30, r0
    lbz r4, 0xe(r4)
    bl fn_80132A38
    lha r6, 0x56(r27)
    mr r3, r28
    mr r5, r26
    mr r7, r29
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    @L_8000F754:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
