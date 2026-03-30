    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    li r3, 0x10
    bl fn_8021C034
    extsb. r0, r3
    bge @L_8025B8A0
    mr r3, r27
    mr r4, r30
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025B884
    li r3, 0x0
    b @L_8025B904
@L_8025B884:
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    li r7, 0x0
    bl fn_8025C264
    mr r31, r3
@L_8025B8A0:
    mr r3, r27
    mr r4, r28
    mr r5, r30
    mr r6, r29
    li r7, 0x10
    li r8, 0x2
    li r9, 0x41
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_8025B8D0
    li r3, 0x0
    b @L_8025B904
@L_8025B8D0:
    li r3, 0x10
    bl fn_8021C034
    extsb. r0, r3
    bge @L_8025B900
    cmpwi r31, 0x0
    bne @L_8025B8F0
    li r3, 0x0
    b @L_8025B904
@L_8025B8F0:
    cmpwi r31, -0x1
    bne @L_8025B900
    li r3, 0x1
    b @L_8025B904
@L_8025B900:
    li r3, 0x1
@L_8025B904:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
