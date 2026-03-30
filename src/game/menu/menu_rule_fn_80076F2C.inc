    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    mr r22, r3
    mr r23, r4
    mr r24, r5
    li r31, 0x0
    li r30, 0x1
    li r29, 0x1
    li r28, 0x0
@L_80076F58:
    mr r3, r22
    rlwinm r4, r28, 0, 16, 31
    bl fn_8012AC08
    mr r26, r3
    li r21, 0x0
    cntlzw r0, r26
    rlwinm r20, r0, 27, 5, 31
    cmpwi r20, 0x0
    bne @L_80076F94
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076F98
@L_80076F94:
    li r21, 0x1
@L_80076F98:
    cmpwi r21, 0x0
    bne @L_80077178
    cmpwi r20, 0x0
    li r20, 0x0
    bne @L_80076FC8
    mr r3, r26
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80076FCC
@L_80076FC8:
    li r20, 0x1
@L_80076FCC:
    cmpwi r20, 0x0
    beq @L_80076FDC
    li r0, 0x0
    b @L_80077010
@L_80076FDC:
    mr r3, r26
    li r20, 0x0
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80077008
    mr r3, r26
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8007700C
@L_80077008:
    li r20, 0x1
@L_8007700C:
    rlwinm r0, r20, 0, 24, 31
@L_80077010:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80077178
    mr r3, r26
    bl fn_8011F4A8
    rlwinm r0, r3, 0, 24, 31
    addi r27, r28, 0x1
    add r31, r31, r0
    b @L_80077170
@L_80077034:
    mr r3, r22
    rlwinm r4, r27, 0, 16, 31
    bl fn_8012AC08
    mr r25, r3
    li r20, 0x0
    cntlzw r0, r25
    rlwinm r21, r0, 27, 5, 31
    cmpwi r21, 0x0
    bne @L_80077070
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80077074
@L_80077070:
    li r20, 0x1
@L_80077074:
    cmpwi r20, 0x0
    bne @L_8007716C
    cmpwi r21, 0x0
    li r21, 0x0
    bne @L_800770A4
    mr r3, r25
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_800770A8
@L_800770A4:
    li r21, 0x1
@L_800770A8:
    cmpwi r21, 0x0
    beq @L_800770B8
    li r0, 0x0
    b @L_800770EC
@L_800770B8:
    mr r3, r25
    li r21, 0x0
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800770E4
    mr r3, r25
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800770E8
@L_800770E4:
    li r21, 0x1
@L_800770E8:
    rlwinm r0, r21, 0, 24, 31
@L_800770EC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8007716C
    mr r3, r26
    bl fn_8011F1A0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_8007713C
    mr r3, r25
    bl fn_8011F1A0
    rlwinm r21, r3, 0, 16, 31
    mr r3, r26
    bl fn_8011F1A0
    rlwinm r0, r3, 0, 16, 31
    subf r3, r0, r21
    subf r0, r21, r0
    or r0, r3, r0
    rlwinm r0, r0, 1, 31, 31
    and r0, r29, r0
    rlwinm r29, r0, 0, 24, 31
@L_8007713C:
    mr r3, r25
    bl fn_8011F5C8
    rlwinm r25, r3, 0, 16, 31
    mr r3, r26
    bl fn_8011F5C8
    rlwinm r0, r3, 0, 16, 31
    subf r3, r0, r25
    subf r0, r25, r0
    or r0, r3, r0
    rlwinm r0, r0, 1, 31, 31
    and r0, r30, r0
    rlwinm r30, r0, 0, 24, 31
@L_8007716C:
    addi r27, r27, 0x1
@L_80077170:
    cmplwi r27, 0x6
    blt @L_80077034
@L_80077178:
    addi r28, r28, 0x1
    cmplwi r28, 0x6
    blt @L_80076F58
    cmpwi r24, 0x2
    beq @L_800771F0
    bge @L_800771A0
    cmpwi r24, 0x0
    beq @L_800771AC
    bge @L_800771C8
    b @L_80077294
@L_800771A0:
    cmpwi r24, 0x4
    bge @L_80077294
    b @L_80077218
@L_800771AC:
    lha r0, 0x4(r23)
    rlwinm r3, r31, 1, 31, 31
    srawi r4, r0, 31
    subfc r0, r31, r0
    adde r0, r4, r3
    rlwinm r3, r0, 0, 24, 31
    b @L_80077298
@L_800771C8:
    lbz r0, 0xc(r23)
    li r3, 0x0
    cmplwi r0, 0x0
    bne @L_800771E4
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800771E8
@L_800771E4:
    li r3, 0x1
@L_800771E8:
    rlwinm r3, r3, 0, 24, 31
    b @L_80077298
@L_800771F0:
    lbz r0, 0xd(r23)
    li r3, 0x0
    cmplwi r0, 0x0
    bne @L_8007720C
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80077210
@L_8007720C:
    li r3, 0x1
@L_80077210:
    rlwinm r3, r3, 0, 24, 31
    b @L_80077298
@L_80077218:
    li r20, 0x0
    li r21, 0x0
    b @L_80077268
@L_80077224:
    mr r3, r22
    mr r4, r21
    bl fn_8012AC08
    cmplwi r3, 0x0
    li r24, 0x0
    beq @L_80077254
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80077258
@L_80077254:
    li r24, 0x1
@L_80077258:
    cmpwi r24, 0x0
    bne @L_80077264
    addi r20, r20, 0x1
@L_80077264:
    addi r21, r21, 0x1
@L_80077268:
    rlwinm r0, r21, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_80077224
    rlwinm r0, r20, 0, 16, 31
    lha r5, 0x6(r23)
    srawi r4, r0, 31
    rlwinm r3, r5, 1, 31, 31
    subfc r0, r5, r0
    adde r0, r4, r3
    rlwinm r3, r0, 0, 24, 31
    b @L_80077298
@L_80077294:
    li r3, 0x0
@L_80077298:
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
