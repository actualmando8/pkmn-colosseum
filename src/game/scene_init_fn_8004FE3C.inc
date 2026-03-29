    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    li r30, 0x0
    lis r3, lbl_803A6AB0@ha
    addi r31, r3, lbl_803A6AB0@l
    b @L_8004FEA0
    @L_8004FE60:
    mr r3, r30
    bl fn_801FBFBC
    clrlwi r0, r30, 16
    lwz r4, 0x0(r3)
    mulli r5, r0, 0x14
    lwz r0, 0x4(r3)
    addi r30, r30, 0x1
    add r5, r31, r5
    stw r4, 0x0(r5)
    stw r0, 0x4(r5)
    lwz r4, 0x8(r3)
    lwz r0, 0xc(r3)
    stw r4, 0x8(r5)
    stw r0, 0xc(r5)
    lwz r0, 0x10(r3)
    stw r0, 0x10(r5)
    @L_8004FEA0:
    clrlwi r0, r30, 16
    cmplwi r0, 0x229
    blt @L_8004FE60
    @L_8004FEAC:
    li r3, 0x8b
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_8004FF48
    li r3, 0x8b
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r30, 0x0
    lis r3, lbl_803A6AB0@ha
    addi r31, r3, lbl_803A6AB0@l
    b @L_8004FF34
    @L_8004FEF4:
    mr r3, r30
    bl fn_801FBFBC
    clrlwi r0, r30, 16
    addi r30, r30, 0x1
    mulli r0, r0, 0x14
    add r5, r31, r0
    lwz r4, 0x0(r5)
    lwz r0, 0x4(r5)
    stw r4, 0x0(r3)
    stw r0, 0x4(r3)
    lwz r4, 0x8(r5)
    lwz r0, 0xc(r5)
    stw r4, 0x8(r3)
    stw r0, 0xc(r3)
    lwz r0, 0x10(r5)
    stw r0, 0x10(r3)
    @L_8004FF34:
    clrlwi r0, r30, 16
    cmplwi r0, 0x229
    blt @L_8004FEF4
    li r3, -0x1
    b @L_8005082C
    @L_8004FF48:
    cmpwi r3, -0x2
    bne @L_8004FF8C
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    beq @L_8004FF78
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_8004FEAC
    @L_8004FF78:
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_80050818
    @L_8004FF8C:
    li r3, 0x8b
    bl fn_801022B8
    cmpwi r3, 0x5dc
    beq @L_80050518
    bge @L_8004FFDC
    cmpwi r3, 0x5d7
    beq @L_80050298
    bge @L_8004FFC4
    cmpwi r3, 0x5d5
    beq @L_80050198
    bge @L_80050218
    cmpwi r3, 0x5d4
    bge @L_80050018
    b @L_8004FEAC
    @L_8004FFC4:
    cmpwi r3, 0x5da
    beq @L_80050418
    bge @L_80050498
    cmpwi r3, 0x5d9
    bge @L_80050398
    b @L_80050318
    @L_8004FFDC:
    cmpwi r3, 0x5e1
    beq @L_80050798
    bge @L_80050000
    cmpwi r3, 0x5df
    beq @L_80050698
    bge @L_80050718
    cmpwi r3, 0x5de
    bge @L_80050618
    b @L_80050598
    @L_80050000:
    cmpwi r3, 0xfd4
    beq @L_80050118
    bge @L_8004FEAC
    cmpwi r3, 0xfd3
    bge @L_80050098
    b @L_8004FEAC
    @L_80050018:
    li r3, 0x0
    li r4, 0x1a
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x44
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050090
    lwz r0, 0x44(r1)
    cmpwi r0, 0xc8
    ble @L_80050064
    li r0, 0xc8
    stw r0, 0x44(r1)
    @L_80050064:
    lwz r0, 0x44(r1)
    cmpwi r0, -0xc8
    bge @L_80050078
    li r0, -0xc8
    stw r0, 0x44(r1)
    @L_80050078:
    lwz r7, 0x44(r1)
    li r3, 0x0
    li r4, 0x1a
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050090:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050098:
    li r3, 0x0
    li r4, 0x1b
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x40
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050110
    lwz r0, 0x40(r1)
    cmpwi r0, 0xc8
    ble @L_800500E4
    li r0, 0xc8
    stw r0, 0x40(r1)
    @L_800500E4:
    lwz r0, 0x40(r1)
    cmpwi r0, -0xc8
    bge @L_800500F8
    li r0, -0xc8
    stw r0, 0x40(r1)
    @L_800500F8:
    lwz r7, 0x40(r1)
    li r3, 0x0
    li r4, 0x1b
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050110:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050118:
    li r3, 0x0
    li r4, 0x1c
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x3c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050190
    lwz r0, 0x3c(r1)
    cmpwi r0, 0xc8
    ble @L_80050164
    li r0, 0xc8
    stw r0, 0x3c(r1)
    @L_80050164:
    lwz r0, 0x3c(r1)
    cmpwi r0, -0xc8
    bge @L_80050178
    li r0, -0xc8
    stw r0, 0x3c(r1)
    @L_80050178:
    lwz r7, 0x3c(r1)
    li r3, 0x0
    li r4, 0x1c
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050190:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050198:
    li r3, 0x0
    li r4, 0x1e
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x38
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050210
    lwz r0, 0x38(r1)
    cmpwi r0, 0xc8
    ble @L_800501E4
    li r0, 0xc8
    stw r0, 0x38(r1)
    @L_800501E4:
    lwz r0, 0x38(r1)
    cmpwi r0, -0xc8
    bge @L_800501F8
    li r0, -0xc8
    stw r0, 0x38(r1)
    @L_800501F8:
    lwz r7, 0x38(r1)
    li r3, 0x0
    li r4, 0x1e
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050210:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050218:
    li r3, 0x0
    li r4, 0x1f
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x34
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050290
    lwz r0, 0x34(r1)
    cmpwi r0, 0xc8
    ble @L_80050264
    li r0, 0xc8
    stw r0, 0x34(r1)
    @L_80050264:
    lwz r0, 0x34(r1)
    cmpwi r0, -0xc8
    bge @L_80050278
    li r0, -0xc8
    stw r0, 0x34(r1)
    @L_80050278:
    lwz r7, 0x34(r1)
    li r3, 0x0
    li r4, 0x1f
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050290:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050298:
    li r3, 0x0
    li r4, 0x20
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x30
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050310
    lwz r0, 0x30(r1)
    cmpwi r0, 0xc8
    ble @L_800502E4
    li r0, 0xc8
    stw r0, 0x30(r1)
    @L_800502E4:
    lwz r0, 0x30(r1)
    cmpwi r0, -0xc8
    bge @L_800502F8
    li r0, -0xc8
    stw r0, 0x30(r1)
    @L_800502F8:
    lwz r7, 0x30(r1)
    li r3, 0x0
    li r4, 0x20
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050310:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050318:
    li r3, 0x0
    li r4, 0x21
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x2c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050390
    lwz r0, 0x2c(r1)
    cmpwi r0, 0xc8
    ble @L_80050364
    li r0, 0xc8
    stw r0, 0x2c(r1)
    @L_80050364:
    lwz r0, 0x2c(r1)
    cmpwi r0, -0xc8
    bge @L_80050378
    li r0, -0xc8
    stw r0, 0x2c(r1)
    @L_80050378:
    lwz r7, 0x2c(r1)
    li r3, 0x0
    li r4, 0x21
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050390:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050398:
    li r3, 0x0
    li r4, 0x22
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x28
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050410
    lwz r0, 0x28(r1)
    cmpwi r0, 0xc8
    ble @L_800503E4
    li r0, 0xc8
    stw r0, 0x28(r1)
    @L_800503E4:
    lwz r0, 0x28(r1)
    cmpwi r0, -0xc8
    bge @L_800503F8
    li r0, -0xc8
    stw r0, 0x28(r1)
    @L_800503F8:
    lwz r7, 0x28(r1)
    li r3, 0x0
    li r4, 0x22
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050410:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050418:
    li r3, 0x0
    li r4, 0x23
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x24
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050490
    lwz r0, 0x24(r1)
    cmpwi r0, 0xc8
    ble @L_80050464
    li r0, 0xc8
    stw r0, 0x24(r1)
    @L_80050464:
    lwz r0, 0x24(r1)
    cmpwi r0, -0xc8
    bge @L_80050478
    li r0, -0xc8
    stw r0, 0x24(r1)
    @L_80050478:
    lwz r7, 0x24(r1)
    li r3, 0x0
    li r4, 0x23
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050490:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050498:
    li r3, 0x0
    li r4, 0x24
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x20
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050510
    lwz r0, 0x20(r1)
    cmpwi r0, 0xc8
    ble @L_800504E4
    li r0, 0xc8
    stw r0, 0x20(r1)
    @L_800504E4:
    lwz r0, 0x20(r1)
    cmpwi r0, -0xc8
    bge @L_800504F8
    li r0, -0xc8
    stw r0, 0x20(r1)
    @L_800504F8:
    lwz r7, 0x20(r1)
    li r3, 0x0
    li r4, 0x24
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050510:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050518:
    li r3, 0x0
    li r4, 0x1d
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x1c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050590
    lwz r0, 0x1c(r1)
    cmpwi r0, 0xc8
    ble @L_80050564
    li r0, 0xc8
    stw r0, 0x1c(r1)
    @L_80050564:
    lwz r0, 0x1c(r1)
    cmpwi r0, -0xc8
    bge @L_80050578
    li r0, -0xc8
    stw r0, 0x1c(r1)
    @L_80050578:
    lwz r7, 0x1c(r1)
    li r3, 0x0
    li r4, 0x1d
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050590:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050598:
    li r3, 0x0
    li r4, 0x2d
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x18
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050610
    lwz r0, 0x18(r1)
    cmpwi r0, 0xc8
    ble @L_800505E4
    li r0, 0xc8
    stw r0, 0x18(r1)
    @L_800505E4:
    lwz r0, 0x18(r1)
    cmpwi r0, -0xc8
    bge @L_800505F8
    li r0, -0xc8
    stw r0, 0x18(r1)
    @L_800505F8:
    lwz r7, 0x18(r1)
    li r3, 0x0
    li r4, 0x2d
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050610:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050618:
    li r3, 0x0
    li r4, 0x25
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x14
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050690
    lwz r0, 0x14(r1)
    cmpwi r0, 0xc8
    ble @L_80050664
    li r0, 0xc8
    stw r0, 0x14(r1)
    @L_80050664:
    lwz r0, 0x14(r1)
    cmpwi r0, -0xc8
    bge @L_80050678
    li r0, -0xc8
    stw r0, 0x14(r1)
    @L_80050678:
    lwz r7, 0x14(r1)
    li r3, 0x0
    li r4, 0x25
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050690:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050698:
    li r3, 0x0
    li r4, 0x26
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x10
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050710
    lwz r0, 0x10(r1)
    cmpwi r0, 0xc8
    ble @L_800506E4
    li r0, 0xc8
    stw r0, 0x10(r1)
    @L_800506E4:
    lwz r0, 0x10(r1)
    cmpwi r0, -0xc8
    bge @L_800506F8
    li r0, -0xc8
    stw r0, 0x10(r1)
    @L_800506F8:
    lwz r7, 0x10(r1)
    li r3, 0x0
    li r4, 0x26
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050710:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050718:
    li r3, 0x0
    li r4, 0x27
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050790
    lwz r0, 0xc(r1)
    cmpwi r0, 0xc8
    ble @L_80050764
    li r0, 0xc8
    stw r0, 0xc(r1)
    @L_80050764:
    lwz r0, 0xc(r1)
    cmpwi r0, -0xc8
    bge @L_80050778
    li r0, -0xc8
    stw r0, 0xc(r1)
    @L_80050778:
    lwz r7, 0xc(r1)
    li r3, 0x0
    li r4, 0x27
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050790:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050798:
    li r3, 0x0
    li r4, 0x28
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80050810
    lwz r0, 0x8(r1)
    cmpwi r0, 0xc8
    ble @L_800507E4
    li r0, 0xc8
    stw r0, 0x8(r1)
    @L_800507E4:
    lwz r0, 0x8(r1)
    cmpwi r0, -0xc8
    bge @L_800507F8
    li r0, -0xc8
    stw r0, 0x8(r1)
    @L_800507F8:
    lwz r7, 0x8(r1)
    li r3, 0x0
    li r4, 0x28
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_80050810:
    bl fn_8001E200
    b @L_8004FEAC
    @L_80050818:
    li r3, 0x8b
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    @L_8005082C:
    lwz r0, 0x54(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
