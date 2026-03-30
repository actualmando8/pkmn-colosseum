    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x48(r1)
    stmw r23, 0x24(r1)
    addi r26, r4, 0x0
    addi r25, r3, 0x0
    addi r27, r5, 0x0
    addi r28, r6, 0x0
    addi r29, r7, 0x0
    addi r3, r26, 0x0
    bl strlen
    cmplwi r3, 0x20
    ble @L_800B47B0
    li r3, -0xc
    b @L_800B4980
    @L_800B47B0:
    addi r3, r25, 0x0
    addi r4, r1, 0x1c
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B47C8
    b @L_800B4980
    @L_800B47C8:
    cmplwi r27, 0x0
    beq @L_800B47E8
    lwz r3, 0x1c(r1)
    lwz r4, 0xc(r3)
    divwu r0, r27, r4
    mullw r0, r0, r4
    subf. r0, r0, r27
    beq @L_800B47F0
    @L_800B47E8:
    li r3, -0x80
    b @L_800B4980
    @L_800B47F0:
    lis r4, 0x1
    subi r30, r4, 0x1
    bl fn_800B1E0C
    addi r31, r3, 0x0
    addi r24, r31, 0x0
    li r23, 0x0
    b @L_800B4894
    @L_800B480C:
    lbz r0, 0x0(r24)
    cmplwi r0, 0xff
    bne @L_800B482C
    clrlwi r0, r30, 16
    cmplwi r0, 0xffff
    bne @L_800B488C
    mr r30, r23
    b @L_800B488C
    @L_800B482C:
    lwz r4, 0x1c(r1)
    addi r3, r24, 0x0
    li r5, 0x4
    lwz r4, 0x10c(r4)
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B488C
    lwz r4, 0x1c(r1)
    addi r3, r24, 0x4
    li r5, 0x2
    lwz r4, 0x10c(r4)
    addi r4, r4, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B488C
    addi r3, r24, 0x0
    addi r4, r26, 0x0
    bl fn_800B4208
    cmpwi r3, 0x0
    beq @L_800B488C
    lwz r3, 0x1c(r1)
    li r4, -0x7
    bl fn_800B02F4
    b @L_800B4980
    @L_800B488C:
    addi r24, r24, 0x40
    addi r23, r23, 0x1
    @L_800B4894:
    clrlwi r0, r23, 16
    cmplwi r0, 0x7f
    blt @L_800B480C
    clrlwi r0, r30, 16
    cmplwi r0, 0xffff
    bne @L_800B48BC
    lwz r3, 0x1c(r1)
    li r4, -0x8
    bl fn_800B02F4
    b @L_800B4980
    @L_800B48BC:
    lwz r3, 0x1c(r1)
    bl fn_800B1A08
    lwz r4, 0x1c(r1)
    lhz r0, 0x6(r3)
    lwz r3, 0xc(r4)
    mullw r0, r3, r0
    cmplw r0, r27
    bge @L_800B48EC
    addi r3, r4, 0x0
    li r4, -0x9
    bl fn_800B02F4
    b @L_800B4980
    @L_800B48EC:
    cmplwi r29, 0x0
    beq @L_800B48FC
    mr r0, r29
    b @L_800B4904
    @L_800B48FC:
    lis r3, fn_800AF280@ha
    addi r0, r3, fn_800AF280@l
    @L_800B4904:
    stw r0, 0xd0(r4)
    clrlslwi r0, r30, 16, 6
    add r7, r31, r0
    lwz r3, 0x1c(r1)
    clrlwi r29, r30, 16
    addi r4, r26, 0x0
    sth r30, 0xbc(r3)
    addi r3, r7, 0x8
    li r5, 0x20
    lwz r6, 0x1c(r1)
    lwz r0, 0xc(r6)
    divwu r0, r27, r0
    sth r0, 0x38(r7)
    bl fn_800CA924
    lwz r4, 0x1c(r1)
    lis r3, fn_800B4644@ha
    addi r5, r3, fn_800B4644@l
    stw r28, 0xc0(r4)
    mr r3, r25
    stw r25, 0x0(r28)
    stw r29, 0x4(r28)
    lwz r4, 0x1c(r1)
    lwz r0, 0xc(r4)
    divwu r4, r27, r0
    bl fn_800B1BAC
    mr. r4, r3
    bge @L_800B497C
    lwz r3, 0x1c(r1)
    bl fn_800B02F4
    b @L_800B4980
    @L_800B497C:
    mr r3, r4
    @L_800B4980:
    lmw r23, 0x24(r1)
    lwz r0, 0x4c(r1)
    addi r1, r1, 0x48
    mtlr r0
    blr
