    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r28, r4
    mr r29, r5
    mr r27, r3
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r4, r28
    li r3, 0x0
    bl fn_801F4354
    mr r0, r3
    mr r3, r27
    mr r30, r0
    mr r4, r28
    mr r5, r29
    bl fn_8024F8B4
    mr. r31, r3
    bgt @L_8024E5EC
    li r3, 0x0
    b @L_8024E67C
@L_8024E5EC:
    mr r3, r28
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    li r0, 0x228
    mr r7, r3
    mr r6, r30
    stw r0, 0xc(r1)
    subi r3, r5, 0x139d
    subi r4, r5, 0x13fc
    subi r5, r5, 0x13fb
    stw r31, 0x10(r1)
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    mr r3, r27
    mr r4, r29
    mr r6, r28
    li r5, 0x1
    bl fn_8024E690
    mr r5, r3
    extsh. r0, r5
    bge @L_8024E658
    li r3, 0x0
    b @L_8024E67C
@L_8024E658:
    lis r4, 0x8037
    extsh r8, r5
    addi r7, r4, 23856
    mr r3, r28
    li r4, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8020505C
    li r3, 0x1
@L_8024E67C:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
