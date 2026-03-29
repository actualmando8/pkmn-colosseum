    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lwz r28, lbl_8047A500(r13)
    bl fn_801D1B4C
    cmpwi r3, 0x2
    beq @L_8004C030
    bge @L_8004BFF0
    cmpwi r3, 0x0
    beq @L_8004C0D0
    bge @L_8004BFFC
    b @L_8004C0D0
    @L_8004BFF0:
    cmpwi r3, 0x4
    bge @L_8004C0D0
    b @L_8004C080
    @L_8004BFFC:
    mr r30, r28
    li r31, 0x0
    b @L_8004C020
    @L_8004C008:
    mr r3, r31
    bl fn_801D1F0C
    clrlwi r0, r3, 16
    addi r31, r31, 0x1
    sth r0, 0x0(r30)
    addi r30, r30, 0x2
    @L_8004C020:
    bl fn_801D1F7C
    cmpw r31, r3
    blt @L_8004C008
    b @L_8004C100
    @L_8004C030:
    bl fn_801D1F7C
    mr r30, r28
    mr r29, r3
    li r31, 0x0
    b @L_8004C05C
    @L_8004C044:
    mr r3, r31
    bl fn_801D1F0C
    clrlwi r0, r3, 16
    addi r31, r31, 0x1
    sth r0, 0x0(r30)
    addi r30, r30, 0x2
    @L_8004C05C:
    cmpw r31, r29
    blt @L_8004C044
    lis r4, fn_8004BF20@ha
    mr r3, r28
    addi r6, r4, fn_8004BF20@l
    li r5, 0x2
    mr r4, r29
    bl fn_800CA620
    b @L_8004C100
    @L_8004C080:
    bl fn_801D1F7C
    mr r29, r28
    mr r31, r3
    li r30, 0x0
    b @L_8004C0AC
    @L_8004C094:
    mr r3, r30
    bl fn_801D1F0C
    clrlwi r0, r3, 16
    addi r30, r30, 0x1
    sth r0, 0x0(r29)
    addi r29, r29, 0x2
    @L_8004C0AC:
    cmpw r30, r31
    blt @L_8004C094
    lis r4, fn_8004BE90@ha
    mr r3, r28
    addi r6, r4, fn_8004BE90@l
    li r5, 0x2
    mr r4, r31
    bl fn_800CA620
    b @L_8004C100
    @L_8004C0D0:
    mr r30, r28
    bl fn_801D1F7C
    subi r29, r3, 0x1
    b @L_8004C0F8
    @L_8004C0E0:
    mr r3, r29
    bl fn_801D1F0C
    clrlwi r0, r3, 16
    subi r29, r29, 0x1
    sth r0, 0x0(r30)
    addi r30, r30, 0x2
    @L_8004C0F8:
    cmpwi r29, 0x0
    bge @L_8004C0E0
    @L_8004C100:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
