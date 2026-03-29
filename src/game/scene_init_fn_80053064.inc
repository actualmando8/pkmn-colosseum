    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    mr r4, r27
    mr r5, r28
    mr r6, r29
    li r3, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800530F8
    lwz r0, 0x8(r1)
    cmpw r0, r30
    ble @L_800530D0
    stw r30, 0x8(r1)
    @L_800530D0:
    lwz r0, 0x8(r1)
    cmpw r0, r31
    bge @L_800530E0
    stw r31, 0x8(r1)
    @L_800530E0:
    lwz r7, 0x8(r1)
    mr r4, r27
    mr r5, r28
    mr r6, r29
    li r3, 0x0
    bl fn_801FAA58
    @L_800530F8:
    bl fn_8001E200
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
