    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r30, r4
    li r27, -0x1
    li r4, 0x0
    bl fn_801040D0
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8000F950
    bl fn_80103FE4
    mr r29, r3
    cmplwi r29, 0x0
    beq @L_8000F950
    mr r3, r26
    mr r4, r30
    bl fn_8001D834
    lha r4, 0x6(r30)
    mr r31, r3
    subi r0, r4, 0x11d9
    cmplwi r0, 0x14
    bgt @L_8000F7FC
    lis r3, jumptable_802E4CD8@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802E4CD8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r27, 0x0
    b @L_8000F7FC
    li r27, 0x1
    b @L_8000F7FC
    li r27, 0x2
    b @L_8000F7FC
    li r27, 0x3
    @L_8000F7FC:
    cmpwi r27, 0x0
    blt @L_8000F950
    mulli r28, r27, 0xc
    add r27, r29, r28
    lwz r0, 0x4(r27)
    cmplwi r0, 0x0
    beq @L_8000F828
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8000F838
    @L_8000F828:
    mr r3, r30
    li r4, 0x0
    bl fn_80109220
    b @L_8000F950
    @L_8000F838:
    lha r0, 0x6(r30)
    cmpwi r0, 0x11e2
    bge @L_8000F858
    cmpwi r0, 0x11dd
    beq @L_8000F950
    bge @L_8000F950
    cmpwi r0, 0x11d9
    b @L_8000F950
    @L_8000F858:
    cmpwi r0, 0x11ea
    bge @L_8000F86C
    cmpwi r0, 0x11e6
    bge @L_8000F898
    b @L_8000F878
    @L_8000F86C:
    cmpwi r0, 0x11ee
    bge @L_8000F950
    b @L_8000F930
    @L_8000F878:
    mr r3, r27
    mr r5, r26
    lhz r6, 0xc(r3)
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_8000F950
    @L_8000F898:
    li r3, 0x197
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r30)
    extsh r3, r3
    mr r5, r31
    subf r3, r3, r0
    li r4, 0x0
    srwi r0, r3, 31
    li r6, 0x197
    add r0, r0, r3
    srawi r0, r0, 1
    extsh r27, r0
    mr r3, r27
    bl fn_800FB680
    add r4, r29, r28
    li r3, 0x34
    lbz r4, 0xf(r4)
    bl fn_80132A38
    lha r6, 0x56(r30)
    mr r5, r27
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    add r4, r29, r28
    li r3, 0x34
    lbz r4, 0xe(r4)
    bl fn_80132A38
    lha r5, 0x54(r30)
    mr r7, r31
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_8000F950
    @L_8000F930:
    lwz r4, 0x4(r27)
    li r3, 0x37
    bl fn_80132A38
    mr r5, r31
    li r3, 0x0
    li r4, 0x0
    li r6, 0xe7
    bl fn_800FB680
    @L_8000F950:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
