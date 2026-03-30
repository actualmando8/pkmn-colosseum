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
    bge @L_8025B988
    mr r3, r27
    mr r4, r30
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025B96C
    li r3, 0x0
    b @L_8025B9EC
@L_8025B96C:
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    li r7, 0x0
    bl fn_8025C264
    mr r31, r3
@L_8025B988:
    mr r3, r27
    mr r4, r28
    mr r5, r30
    mr r6, r29
    li r7, 0x10
    li r8, 0x1
    li r9, 0x41
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_8025B9B8
    li r3, 0x0
    b @L_8025B9EC
@L_8025B9B8:
    li r3, 0x10
    bl fn_8021C034
    extsb. r0, r3
    bge @L_8025B9E8
    cmpwi r31, 0x0
    bne @L_8025B9D8
    li r3, 0x0
    b @L_8025B9EC
@L_8025B9D8:
    cmpwi r31, -0x1
    bne @L_8025B9E8
    li r3, 0x1
    b @L_8025B9EC
@L_8025B9E8:
    li r3, 0x1
@L_8025B9EC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
