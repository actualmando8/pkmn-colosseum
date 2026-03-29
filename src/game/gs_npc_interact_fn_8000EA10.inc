    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    li r30, -0x1
    li r4, 0x0
    bl fn_801040D0
    lha r4, 0x6(r29)
    subi r0, r4, 0x1215
    cmplwi r0, 0x17
    bgt @L_8000EA94
    lis r4, jumptable_802E4C20@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_802E4C20@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r30, 0x0
    b @L_8000EA94
    li r30, 0x1
    b @L_8000EA94
    li r30, 0x2
    b @L_8000EA94
    li r30, 0x3
    b @L_8000EA94
    li r30, 0x4
    b @L_8000EA94
    li r30, 0x5
    @L_8000EA94:
    cmpwi r30, 0x0
    blt @L_8000ED14
    clrlwi r30, r30, 16
    cmplwi r30, 0x6
    blt @L_8000EAB0
    li r31, 0x0
    b @L_8000EB10
    @L_8000EAB0:
    cmplwi r3, 0x0
    bne @L_8000EAC0
    li r3, 0x0
    bl fn_801F2A7C
    @L_8000EAC0:
    cmplwi r3, 0x0
    bne @L_8000EAD0
    li r31, 0x0
    b @L_8000EB10
    @L_8000EAD0:
    mr r4, r30
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8000EAE8
    li r31, 0x0
    b @L_8000EB10
    @L_8000EAE8:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000EB10
    li r31, 0x0
    @L_8000EB10:
    cmplwi r31, 0x0
    beq @L_8000ED14
    mr r3, r28
    mr r4, r29
    bl fn_8001D834
    lha r0, 0x6(r29)
    mr r30, r3
    cmpwi r0, 0x1221
    bge @L_8000EB48
    cmpwi r0, 0x121b
    bge @L_8000EB84
    cmpwi r0, 0x1215
    bge @L_8000EB5C
    b @L_8000ED14
    @L_8000EB48:
    cmpwi r0, 0x122d
    bge @L_8000ED14
    cmpwi r0, 0x1227
    bge @L_8000ECD4
    b @L_8000EC70
    @L_8000EB5C:
    mr r3, r31
    li r4, 0x1
    bl fn_8001D624
    mr r6, r3
    mr r5, r28
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_8000ED14
    @L_8000EB84:
    li r3, 0x1a8
    bl fn_800FA444
    srwi r0, r3, 16
    mr r5, r30
    extsh r4, r0
    li r3, 0x0
    addi r0, r4, 0x2
    li r4, 0x0
    extsh r28, r0
    li r6, 0x1a8
    bl fn_800FB680
    li r3, 0x197
    bl fn_800FA444
    lha r0, 0x54(r29)
    srwi r3, r3, 16
    extsh r3, r3
    mr r5, r30
    subf r0, r28, r0
    li r4, 0x0
    subf r3, r3, r0
    li r6, 0x197
    srwi r0, r3, 31
    add r0, r0, r3
    srawi r0, r0, 1
    extsh r0, r0
    add r28, r28, r0
    mr r3, r28
    bl fn_800FB680
    mr r3, r31
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r6, 0x56(r29)
    mr r7, r30
    extsh r5, r28
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    mr r3, r31
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r29)
    mr r7, r30
    lha r6, 0x56(r29)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    b @L_8000ED14
    @L_8000EC70:
    li r3, 0x1a7
    bl fn_800FA444
    srwi r0, r3, 16
    mr r5, r30
    extsh r28, r0
    li r3, 0x0
    li r4, 0x0
    li r6, 0x1a7
    bl fn_800FB680
    mr r3, r31
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 24
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r29)
    mr r3, r28
    lha r6, 0x56(r29)
    mr r7, r30
    li r4, 0x0
    li r8, 0xd2
    bl fn_800FBB34
    b @L_8000ED14
    @L_8000ECD4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    lha r5, 0x54(r29)
    mr r7, r30
    lha r6, 0x56(r29)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xe7
    bl fn_800FBB34
    @L_8000ED14:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
